#include "r_archive.h"
#include <Rcpp.h>
#include <fcntl.h>

char * basename (const char * filename)
{
  char *p = strrchr (filename, '/');
  return p ? p + 1 : (char *) filename;
}

/* callback function to store received data */
static size_t rchive_write_data(const void *contents, size_t sz, size_t n, Rconnection ctx) {
  rchive* r = (rchive*) ctx->private_ptr;

  size_t realsize = sz * n;
  archive_write_data(r->ar, contents, realsize);
  r->size += realsize;

  return n;
}

std::string scratch_file(const char* filename) {
  static Rcpp::Function tempdir("tempdir", R_BaseEnv);
  std::string out = std::string(CHAR(STRING_ELT(tempdir(), 0))) + '/' + basename(filename);
  return out;
}

static Rboolean rchive_write_open(Rconnection con) {
  rchive *r = (rchive *) con->private_ptr;

  r->ar = archive_write_disk_new();

  r->entry = archive_entry_new();

  archive_entry_set_pathname(r->entry, scratch_file(r->filename).c_str());
  archive_entry_set_filetype(r->entry, AE_IFREG);
  archive_entry_set_perm(r->entry, 0644);
  archive_write_header(r->ar, r->entry);

  con->isopen = TRUE;
  return TRUE;
}

/* This function closes the temporary scratch file, then writes the actual
 * archive file based on the archive filename given and then unlinks the
 * scratch file */
void rchive_write_close(Rconnection con) {
  char buf[8192];
  size_t bytes_read;
  rchive *r = (rchive *) con->private_ptr;

  /* Close scratch file */
  archive_write_finish_entry(r->ar);
  archive_write_close(r->ar);
  archive_write_free(r->ar);
  archive_entry_free(r->entry);

  /* Write scratch file to archive */
  struct archive *in;
  struct archive *out;
  struct archive_entry *entry;
  in = archive_read_disk_new();
  entry = archive_entry_new();

  std::string scratch = scratch_file(r->filename);
  int fd = open(scratch.c_str(), O_RDONLY);
  if (fd < 0) {
    Rcpp::stop("Could not open scratch file");
  }
  archive_read_disk_entry_from_file(in, entry, fd, NULL);
  archive_entry_set_pathname(entry, r->filename);

  out = archive_write_new();
  /* Set archive format and filter according to output file extension.
   * If it fails, set default format. Platform depended function.
   * See supported formats in archive_write_set_format_filter_by_ext.c */
  if (archive_write_set_format_filter_by_ext(out, r->archive_filename) != ARCHIVE_OK)  {
    archive_write_add_filter_gzip(out);
    archive_write_set_format_ustar(out);
  }
  archive_write_open_filename(out, r->archive_filename);
  archive_write_header(out, entry);

  while ((bytes_read = read(fd, buf, sizeof(buf))) > 0) {
    archive_write_data(out, buf, bytes_read);
  }
  close(fd);
  archive_write_free(out);
  archive_entry_free(entry);
  archive_read_free(in);

  unlink(scratch.c_str());
  con->isopen = FALSE;
}

void rchive_write_destroy(Rconnection con) {
  rchive *r = (rchive *) con->private_ptr;

  /* free the handle connection */
  free(r->archive_filename);
  free(r->filename);
  free(r);
}

// [[Rcpp::export]]
SEXP write_connection(const std::string & archive_filename, const std::string & filename, size_t sz = 16384) {
  Rconnection con;
  SEXP rc = PROTECT(R_new_custom_connection("input", "wb", "archive", &con));

  /* Setup archive */
  rchive *r = (rchive *) malloc(sizeof(rchive));
  //r->limit = sz;
  //r->buf = (char *) malloc(r->limit);
  //r->cur = r->buf;
  //r->size = 0;

  r->archive_filename = (char *) malloc(strlen(archive_filename.c_str()) + 1);
  strcpy(r->archive_filename, archive_filename.c_str());

  r->filename = (char *) malloc(strlen(filename.c_str()) + 1);
  strcpy(r->filename, filename.c_str());

  /* set connection properties */
  con->incomplete = TRUE;
  con->private_ptr = r;
  con->canread = FALSE;
  con->canseek = FALSE;
  con->canwrite = TRUE;
  con->isopen = FALSE;
  con->blocking = TRUE;
  con->text = FALSE;
  con->open = rchive_write_open;
  con->close = rchive_write_close;
  con->destroy = rchive_write_destroy;
  con->write = rchive_write_data;

  UNPROTECT(1);
  return rc;
}

// [[Rcpp::export]]
SEXP write_files(const std::string & archive_filename, Rcpp::CharacterVector files, size_t sz = 16384) {
  struct archive *a;
  struct archive_entry *entry;
  struct stat st;
  char buff[8192];
  int len;
  int fd;

  a = archive_write_new();
  /* Set archive format and filter according to output file extension.
   * If it fails, set default format. Platform depended function.
   * See supported formats in archive_write_set_format_filter_by_ext.c */
  if (archive_write_set_format_filter_by_ext(a, archive_filename.c_str()) != ARCHIVE_OK)  {
    archive_write_add_filter_gzip(a);
    archive_write_set_format_ustar(a);
  }

  archive_write_open_filename(a, archive_filename.c_str());
  for (int i = 0; i < files.size(); ++i) {
    stat(files[i], &st);
    entry = archive_entry_new();
    archive_entry_copy_stat(entry, &st);
    archive_entry_set_pathname(entry, files[i]);
    archive_write_header(a, entry);
    if ((fd = open(files[i], O_RDONLY)) != -1) {
      len = read(fd, buff, sizeof(buff));
      while (len > 0) {
        archive_write_data(a, buff, len);
        len = read(fd, buff, sizeof(buff));
      }
      close(fd);
    }
    archive_entry_free(entry);
  }
  archive_write_free(a);

  return R_NilValue;
}

