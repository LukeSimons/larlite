#ifndef NOISEFILTER_TYPES_CXX
#define NOISEFILTER_TYPES_CXX

#include "NoiseFilterTypes.h"

namespace ub_noise_filter {

float getCorrelation(const std::vector<float> & _input1, const std::vector<float> & _input2)
{

  if (_input1.size() != _input2.size()) {
    std::cerr << "ERROR: vector sizes do not match" << std::endl;
    return 0.0;
  }

  float rms_1  = 0.0;
  float rms_2  = 0.0;
  float mean_1 = 0.0;
  float mean_2 = 0.0;

  for (unsigned int i = 0; i < _input1.size(); i++) {
    mean_1 += _input1.at(i);
    mean_2 += _input2.at(i);
  }

  mean_1 /= _input1.size();
  mean_2 /= _input2.size();

  float corr = 0.0;

  for (unsigned int i = 0; i < _input1.size(); i++) {
    rms_1 += pow(mean_1 - _input1.at(i), 2);
    rms_2 += pow(mean_2 - _input2.at(i), 2);
    corr += (mean_1 - _input1.at(i)) * (mean_2 - _input2.at(i));
  }

  rms_1 = sqrt(rms_1 / _input1.size());
  rms_2 = sqrt(rms_2 / _input2.size());

  corr /= (rms_1 * rms_2 * _input1.size());
  return corr;

}



float getMode(const std::vector<float> & _input,
              float lowbin,
              float highbin,
              int n_bins) {

  std::vector<int> histogram;
  std::vector<float> lowerBounds;
  std::vector<float> upperBounds;

  histogram.resize(n_bins);
  lowerBounds.resize(n_bins);
  upperBounds.resize(n_bins);

  float bin_width = (highbin - lowbin) / n_bins;

  for (int i = 0; i < n_bins; i ++) {
    lowerBounds[i] = lowbin + i * bin_width;
    upperBounds[i] = lowbin + (i + 1) * bin_width;
  }

  // Fill in the values from the input vector to the histogram:
  //
  for (auto & val : _input) {
    int bin = 0;
    if (val < lowerBounds[0] || val > upperBounds.back())
      continue;
    while (bin < n_bins) {
      if (val > lowerBounds[bin] && val < upperBounds[bin]) {
        histogram.at(bin) ++;
        break;
      }
      else {
        bin ++;
      }
    }
  }


  // Get the most probable value:
  int max_count = 0;
  int max_bin = 0;
  for (size_t i = 0; i < histogram.size(); i ++) {
    if (histogram[i] > max_count) {
      max_count = histogram[i];
      max_bin = i;
    }
  }

  return (upperBounds[max_bin + 1] + lowerBounds[max_bin]) / 2;
}

float getMedian( std::vector<float> & _input) {


  if (_input.size() % 2 == 1) {
    std::nth_element(_input.begin(),
                     _input.begin() + _input.size() / 2,
                     _input.end());
    return  _input.at(_input.size() / 2);

  }
  else if (_input.size() > 2) {
    std::nth_element(_input.begin(),
                     _input.begin() + _input.size() / 2,
                     _input.end());
    std::nth_element(_input.begin(),
                     _input.begin() + _input.size() / 2 - 1,
                     _input.end());
    return 0.5 * (_input.at(_input.size() / 2) +
                  _input.at(_input.size() / 2 - 1));
  }

  return 0.0;
}

float getMean(const std::vector<float> & _input) {
  if (_input.size() == 0) {
    std::cerr << "ERROR: can't compute mean of zero size vector." << std::endl;
    return 0.0;
  }

  float rms  = 0.0;
  float mean = 0.0;

  for (unsigned int i = 0; i < _input.size(); i++) {
    mean += _input.at(i);
  }

  mean /= _input.size();
  return mean;
}


detPropFetcher::detPropFetcher() {
  _wire_lengths.resize(3);
  _wire_lengths[0].resize(2400);
  _wire_lengths[1].resize(2400);
  _wire_lengths[2].resize(3456);

  _wire_scales.resize(3);
  _wire_scales[0].resize(2400);
  _wire_scales[1].resize(2400);
  _wire_scales[2].resize(3456);

  auto geom = larutil::Geometry::GetME();


  for (int wire = 0; wire < 2400; wire ++) {
    double xyzStart[3];
    double xyzEnd[3];


    geom ->WireEndPoints(0, wire,
                         xyzStart,
                         xyzEnd) ;

    _wire_lengths[0][wire] = sqrt(
                               (xyzStart[0] - xyzEnd[0]) * (xyzStart[0] - xyzEnd[0]) +
                               (xyzStart[1] - xyzEnd[1]) * (xyzStart[1] - xyzEnd[1]) +
                               (xyzStart[2] - xyzEnd[2]) * (xyzStart[2] - xyzEnd[2])
                             );

    _wire_scales[0][wire] = _wire_lengths[0][wire] / 456.9;

    geom ->WireEndPoints(1, wire,
                         xyzStart,
                         xyzEnd) ;

    _wire_lengths[1][wire] = sqrt(
                               (xyzStart[0] - xyzEnd[0]) * (xyzStart[0] - xyzEnd[0]) +
                               (xyzStart[1] - xyzEnd[1]) * (xyzStart[1] - xyzEnd[1]) +
                               (xyzStart[2] - xyzEnd[2]) * (xyzStart[2] - xyzEnd[2])
                             );

    _wire_scales[1][wire] = _wire_lengths[1][wire] / 456.9;

  }

  for (int wire = 0; wire < 3456; wire ++) {
    double xyzStart[3];
    double xyzEnd[3];


    geom ->WireEndPoints(2, wire,
                         xyzStart,
                         xyzEnd) ;

    _wire_lengths[2][wire] = sqrt(
                               (xyzStart[0] - xyzEnd[0]) * (xyzStart[0] - xyzEnd[0]) +
                               (xyzStart[1] - xyzEnd[1]) * (xyzStart[1] - xyzEnd[1]) +
                               (xyzStart[2] - xyzEnd[2]) * (xyzStart[2] - xyzEnd[2])
                             );

    _wire_scales[2][wire] = _wire_lengths[2][wire] / 456.9;

  }

}
std::vector<std::vector<float> > service_board_block(int plane, int block) {

  std::vector< std::vector<float> > ret;

  ret.resize(3);

  int second_block;

  if (plane == 0) {

    // Reject if block is too high:
    if (block > 50) {
      std::cerr << "Requested block is too high." << std::endl;
      return ret;
    }

    // Catch the edge cases:
    if (block > 36) {

    }
    else {

      // Figure out which pair of blocks this goes with in the same plane:
      if (block % 2 ) {
        second_block = block + 1;
      }
      else {
        second_block = block - 1;
      }

      if (block > second_block){
        std::swap(block, second_block);
      }

      // Now build up the vectors:
      // This is plane 0, so that's easy:
      ret[0].push_back(block);
      ret[0].push_back(second_block);

    }

  }
  else if (plane == 1) {

    // Reject if block is too high:
    if (block > 50) {
      std::cerr << "Requested block is too high." << std::endl;
      return ret;
    }


  }
  else if (plane == 2) {

    // Reject if block is too high:
    if (block > 36) {
      std::cerr << "Requested block is too high." << std::endl;
      return ret;
    }

  }


  return ret;
}

unsigned int detPropFetcher::n_wires(unsigned int plane) {
  if (plane == 0)
    return 2400;
  else if (plane == 1)
    return 2400;
  else
    return 3456;
}
unsigned int detPropFetcher::n_planes() {
  return 3;
}

double detPropFetcher::wire_length(unsigned int plane, unsigned int wire) {

  return _wire_lengths[plane][wire];
}

double detPropFetcher::wire_scale(unsigned int plane, unsigned int wire) {
  return _wire_scales[plane][wire];
}


}




#endif
