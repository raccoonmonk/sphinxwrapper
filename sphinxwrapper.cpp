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

  rv = ps_decode_raw(ps, fh, "goforward", -1);
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
