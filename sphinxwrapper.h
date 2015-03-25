#pragma once

#include <string>

class SphinxWrapper
{
public:

  /*!
   * \brief SphinxWrapper
   * \param modeldir Path to the speech model directory
   */
  explicit SphinxWrapper(const char * modeldir);
  ~SphinxWrapper();

  /*!
   * \brief recognize speech from an audio file
   * \param filename name of the audio file
   * \return std::string with recognized text
   */
  std::string recognize(const std::string & filename);

  /*!
   * \brief recognize speech from a block of memory
   * \param buf pointer to a data block
   * \param size size of dlock in bytes
   * \return std::string with recognized text
   */
  std::string recognize(const char * buf, int size);

private:
  bool init(); //!< Init ps_decoder_t*
  std::string & getResultAndClean(std::string & str);

  char * m_modeldir; //!< Path to model directory
  void * m_config;
  void * m_ps;
};
