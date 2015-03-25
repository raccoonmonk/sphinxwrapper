#include "sphinxwrapper.h"
#include <pocketsphinx.h>
#include <err.h>

SphinxWrapper::SphinxWrapper(const char * modeldir) {
  m_modeldir = new char[strlen(modeldir)];
  strcpy(m_modeldir, modeldir);
  m_config = NULL;
  m_ps = NULL;
  err_set_logfp(NULL); //disable logging
}

SphinxWrapper::~SphinxWrapper() {
  delete [] m_modeldir;
}

std::string SphinxWrapper::recognize(const std::string & filename) {
  std::string result("ERROR. Something went wrong.");

  int32 score = 0;
  const char *hyp = NULL, *uttid = NULL;
  int rv = 0;

  if (init())
    return getResultAndClean(result);

  ps_decoder_t * ps = reinterpret_cast<ps_decoder_t *>(m_ps);
  FILE * fh;
  fh = fopen(filename.data(), "rb");
  if(fh == NULL)
    return getResultAndClean(result);

  rv = ps_decode_raw(ps, fh, NULL, -1);
  if(rv < 0)
    return getResultAndClean(result);

  hyp = ps_get_hyp(ps, &score, &uttid);
  if (hyp == NULL)
    return getResultAndClean(result);

  fclose(fh);
  result = std::string(hyp);
  return getResultAndClean(result);
}

std::string SphinxWrapper::recognize(const char * buf, int size) {
  std::string result("ERROR. Something went wrong.");

  int32 score = 0;
  const char *hyp = NULL, *uttid = NULL;
  int rv = 0;

  if (init())
    return getResultAndClean(result);

  ps_decoder_t * ps = reinterpret_cast<ps_decoder_t *>(m_ps);
  rv = ps_start_utt(ps, NULL);
  if (rv < 0)
    return getResultAndClean(result);

  rv = ps_process_raw(ps, (const int16 *)buf, size/2, FALSE, FALSE);
  rv = ps_end_utt(ps);
  if (rv < 0)
    return getResultAndClean(result);

  hyp = ps_get_hyp(ps, &score, &uttid);
  if (hyp == NULL)
    return getResultAndClean(result);

  result = std::string(hyp);
  return getResultAndClean(result);
}

inline bool SphinxWrapper::init() {
  ps_decoder_t * ps = NULL;
  cmd_ln_t * config = cmd_ln_init(NULL, ps_args(), TRUE,
           "-hmm", m_modeldir,
           "-lm", std::string(m_modeldir).append("/turtle.DMP").data(),
           "-dict", std::string(m_modeldir).append("/turtle.dic").data(),
           NULL);
  if (config == NULL)
      return 1;
  m_config = (void *)config;
  ps = ps_init(config);
  if (ps == NULL)
    return 1;
  m_ps = (void *)ps;
  return 0;
}

std::string & SphinxWrapper::getResultAndClean(std::string & str) {
  ps_decoder_t * ps = reinterpret_cast<ps_decoder_t *>(m_ps);
  cmd_ln_t * config = reinterpret_cast<cmd_ln_t *>(m_config);
  if(ps != NULL)
    ps_free(ps);
  if(config != NULL)
    cmd_ln_free_r(config);
  m_ps = NULL;
  m_config = NULL;

  return str;
}
