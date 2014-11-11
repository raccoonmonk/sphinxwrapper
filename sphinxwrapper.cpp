#include <iostream>
#include "sphinxwrapper.h"
#define MODELDIR "./model"

SphinxWrapper::SphinxWrapper()
{
  config = cmd_ln_init(NULL, ps_args(), TRUE,
           "-hmm", MODELDIR,
           "-lm", MODELDIR "/turtle.DMP",
           "-dict", MODELDIR "/turtle.dic",
           NULL);
}
SphinxWrapper::~SphinxWrapper() {
  fclose(fh);
  ps_free(ps);
}

std::string SphinxWrapper::recognize(const std::string & filename) {
  if (config == NULL)
      return NULL;
    ps = ps_init(config);
    if (ps == NULL)
      return NULL;

    fh = fopen(filename.data(), "rb");
    if (fh == NULL) {
      return std::string("fail to open file");
    }

    rv = ps_decode_raw(ps, fh, "goforward", -1);
    if (rv < 0)
      return NULL;
    hyp = ps_get_hyp(ps, &score, &uttid);
    if (hyp == NULL)
      return NULL;
    return std::string(hyp);
//    std::cout << "Recognized:" << hyp << std::endl;

          fseek(fh, 0, SEEK_SET);
          rv = ps_start_utt(ps, "goforward");
    if (rv < 0)
      return NULL;
          while (!feof(fh)) {
              size_t nsamp;
              nsamp = fread(buf, 2, 512, fh);
              rv = ps_process_raw(ps, buf, nsamp, FALSE, FALSE);
          }
          rv = ps_end_utt(ps);
    if (rv < 0)
      return NULL;
    hyp = ps_get_hyp(ps, &score, &uttid);
    if (hyp == NULL)
      return NULL;
  return std::string(hyp);
//  std::cout << "Recognized:" << hyp << std::endl;
}
