/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef ANALYSIS_PSUADEDACEALGORITHMOPTIONS_IMPL_HPP
#define ANALYSIS_PSUADEDACEALGORITHMOPTIONS_IMPL_HPP

#include "AnalysisAPI.hpp"
#include "DakotaAlgorithmOptions_Impl.hpp"

#include "PSUADEDaceAlgorithmOptions.hpp"

namespace openstudio {
namespace analysis {

class Problem;

namespace detail {

  /** PSUADEDaceAlgorithmOptions_Impl is a DakotaAlgorithmOptions_Impl that is the implementation 
   *  class for PSUADEDaceAlgorithmOptions.*/
  class ANALYSIS_API PSUADEDaceAlgorithmOptions_Impl : public DakotaAlgorithmOptions_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    PSUADEDaceAlgorithmOptions_Impl();

    /** Constructor provided for deserialization; not for general use. */
    explicit PSUADEDaceAlgorithmOptions_Impl(const std::vector<Attribute>& options);

    virtual ~PSUADEDaceAlgorithmOptions_Impl() {}

    virtual AlgorithmOptions clone() const override;

    //@}

    /** @name Getters and Queries */
    //@{

    /** Returns the explict pseudo-random number generator seed if it exists, evaluates to false 
     *  otherwise. */
    boost::optional<int> seed() const;

    /** Returns the user-specified number of samples to be used if it exists, evaluates to false
     *  otherwise. */
    boost::optional<int> samples() const;

    /** Returns the user-specified number of partitions to be used if it exists, evaluates to false
     *  otherwise. */
    boost::optional<int> partitions() const;

    //@}
    /** @name Setters */
    //@{

    /** Seed value must be greater than zero. */
    bool setSeed(int value);
    
    void clearSeed();    

    /** The number of samples must be greater than zero. This value should be an integer multiple of
     *  (number of continuous design variables + 1), but if will automatically adjusted by Dakota if
     *  misspecified.*/
    bool setSamples(int value);
    
    void clearSamples();    

    /** The number of partitions must be greater than zero. This value should be odd, though Dakota
     *  will automatically correct the value if it is not odd. */
    bool setPartitions(int value);

    void clearPartitions();

    //@}
    /** @name Absent or Protected in Public Class */
    //@{

    static PSUADEDaceAlgorithmOptions fromVariant(const QVariant& variant, const VersionString& version);

    //@}
   private:
    REGISTER_LOGGER("openstudio.analysis.PSUADEDaceAlgorithmOptions");
  };

} // detail

} // analysis
} // openstudio

#endif // ANALYSIS_PSUADEDACEALGORITHMOPTIONS_IMPL_HPP
