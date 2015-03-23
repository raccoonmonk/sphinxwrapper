#include "sphinxwrapper.h"
#include <pocketsphinx.h>
#include <err.h>

SphinxWrapper::SphinxWrapper(const char * modeldir)
{
  m_modeldir = new char[strlen(modeldir)];
  strcpy(m_modeldir, modeldir);
}

SphinxWrapper::~SphinxWrapper() {
  delete [] m_modeldir;
}

std::string SphinxWrapper::recognize(const std::string & filename) {
  std::string result("ERROR. Something went wrong.");

  ps_decoder_t * ps = NULL;
  int32 score = 0;
  const char *hyp = NULL, *uttid = NULL;
  int rv = 0;

  if (init((void * &)ps))
    return result;

  FILE * fh;
  fh = fopen(filename.data(), "rb");
  if(fh == NULL)
    return result;

  rv = ps_decode_raw(ps, fh, NULL, -1);
  if(rv < 0)
    return result;
  hyp = ps_get_hyp(ps, &score, &uttid);
  if (hyp == NULL)
    return result;

  fclose(fh);
  result = std::string(hyp);
  ps_free(ps);
  return result;
}

std::string SphinxWrapper::recognize(const char * buf, int size) {
  std::string result("ERROR. Something went wrong.");

  ps_decoder_t * ps = NULL;
  int32 score = 0;
  const char *hyp = NULL, *uttid = NULL;
  int rv = 0;

  if (init((void * &)ps))
    return result;

  rv = ps_start_utt(ps, NULL);
    if (rv < 0)
      return result;
  rv = ps_process_raw(ps, (const int16 *)buf, size/2, FALSE, FALSE);
  rv = ps_end_utt(ps);
  if (rv < 0)
    return result;
  hyp = ps_get_hyp(ps, &score, &uttid);
  if (hyp == NULL)
    return result;

  result = std::string(hyp);
  ps_free(ps);
  return result;
}

inline bool SphinxWrapper::init(void * & ps_ptr) {
  ps_decoder_t * ps = NULL;
  err_set_logfp(NULL); //disable logging
  cmd_ln_t * config = cmd_ln_init(NULL, ps_args(), TRUE,
           "-hmm", m_modeldir,
           "-lm", std::string(m_modeldir).append("/turtle.DMP").data(),
           "-dict", std::string(m_modeldir).append("/turtle.dic").data(),
           NULL);
  if (config == NULL)
      return 1;
  ps = ps_init(config);
  if (ps == NULL)
    return 1;
  ps_ptr = (void *)ps;
  return 0;
}
