#include <iostream>
#include "sphinxwrapper.h"
#define MODELDIR "../sphinxwrapper/model"

SphinxWrapper::SphinxWrapper()
{
}

std::string SphinxWrapper::recognize(const std::string & filename) {
  std::string result("ERROR. Something went wrong.");
  config = cmd_ln_init(NULL, ps_args(), TRUE,
           "-hmm", MODELDIR,
           "-lm", MODELDIR "/turtle.DMP",
           "-dict", MODELDIR "/turtle.dic",
           NULL);
  if (config == NULL)
      return result;
  ps = ps_init(config);
  if (ps == NULL)
    return result;

  fh = fopen(filename.data(), "rb");
  if(fh == NULL)
    return result;

  rv = ps_decode_raw(ps, fh, NULL, -1);
  if(rv < 0)
    return result;
  hyp = ps_get_hyp(ps, &score, &uttid);
  if (hyp == NULL)
    return result;

  result = std::string(hyp);
  fclose(fh);
  ps_free(ps);
  return result;
}

std::string SphinxWrapper::recognize(const char * buf, int size) {
  std::string result("ERROR. Something went wrong.");
  config = cmd_ln_init(NULL, ps_args(), TRUE,
           "-hmm", MODELDIR,
           "-lm", MODELDIR "/turtle.DMP",
           "-dict", MODELDIR "/turtle.dic",
           NULL);
  if (config == NULL)
      return result;
  ps = ps_init(config);
  if (ps == NULL)
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
