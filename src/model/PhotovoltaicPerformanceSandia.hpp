/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef MODEL_PHOTOVOLTAICPERFORMANCESANDIA_HPP
#define MODEL_PHOTOVOLTAICPERFORMANCESANDIA_HPP

#include <model/ModelAPI.hpp>
#include "PhotovoltaicPerformance.hpp"

namespace openstudio {

namespace model {

namespace detail {

  class PhotovoltaicPerformanceSandia_Impl;

} // detail

/** PhotovoltaicPerformanceSandia is a PhotovoltaicPerformance that wraps the OpenStudio IDD object 'OS:PhotovoltaicPerformance:Sandia'. */
class MODEL_API PhotovoltaicPerformanceSandia : public PhotovoltaicPerformance {
 public:
  /** @name Constructors and Destructors */
  //@{

  // Defaults all characteristics to one random entry in the Sandia Database
  explicit PhotovoltaicPerformanceSandia(const Model& model);

  explicit PhotovoltaicPerformanceSandia(const Model& model,
    double activeArea,
    int numberofCellsinSeries,
    int numberofCellsinParallel,
    double shortCircuitCurrent,
    double openCircuitVoltage,
    double currentatMaximumPowerPoint,
    double voltageatMaximumPowerPoint,
    double sandiaDatabaseParameteraIsc,
    double sandiaDatabaseParameteraImp,
    double sandiaDatabaseParameterc0,
    double sandiaDatabaseParameterc1,
    double sandiaDatabaseParameterBVoc0,
    double sandiaDatabaseParametermBVoc,
    double sandiaDatabaseParameterBVmp0,
    double sandiaDatabaseParametermBVmp,
    double diodeFactor,
    double sandiaDatabaseParameterc2,
    double sandiaDatabaseParameterc3,
    double sandiaDatabaseParametera0,
    double sandiaDatabaseParametera1,
    double sandiaDatabaseParametera2,
    double sandiaDatabaseParametera3,
    double sandiaDatabaseParametera4,
    double sandiaDatabaseParameterb0,
    double sandiaDatabaseParameterb1,
    double sandiaDatabaseParameterb2,
    double sandiaDatabaseParameterb3,
    double sandiaDatabaseParameterb4,
    double sandiaDatabaseParameterb5,
    double sandiaDatabaseParameterDeltaTc,
    double sandiaDatabaseParameterfd,
    double sandiaDatabaseParametera,
    double sandiaDatabaseParameterb,
    double sandiaDatabaseParameterc4,
    double sandiaDatabaseParameterc5,
    double sandiaDatabaseParameterIx0,
    double sandiaDatabaseParameterIxx0,
    double sandiaDatabaseParameterc6,
    double sandiaDatabaseParameterc7
  );
  explicit PhotovoltaicPerformanceSandia(const Model& model);

  virtual ~PhotovoltaicPerformanceSandia() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  double activeArea() const;

  int numberofCellsinSeries() const;

  int numberofCellsinParallel() const;

  double shortCircuitCurrent() const;

  double openCircuitVoltage() const;

  double currentatMaximumPowerPoint() const;

  double voltageatMaximumPowerPoint() const;

  double sandiaDatabaseParameteraIsc() const;

  double sandiaDatabaseParameteraImp() const;

  double sandiaDatabaseParameterc0() const;

  double sandiaDatabaseParameterc1() const;

  double sandiaDatabaseParameterBVoc0() const;

  double sandiaDatabaseParametermBVoc() const;

  double sandiaDatabaseParameterBVmp0() const;

  double sandiaDatabaseParametermBVmp() const;

  double diodeFactor() const;

  double sandiaDatabaseParameterc2() const;

  double sandiaDatabaseParameterc3() const;

  double sandiaDatabaseParametera0() const;

  double sandiaDatabaseParametera1() const;

  double sandiaDatabaseParametera2() const;

  double sandiaDatabaseParametera3() const;

  double sandiaDatabaseParametera4() const;

  double sandiaDatabaseParameterb0() const;

  double sandiaDatabaseParameterb1() const;

  double sandiaDatabaseParameterb2() const;

  double sandiaDatabaseParameterb3() const;

  double sandiaDatabaseParameterb4() const;

  double sandiaDatabaseParameterb5() const;

  double sandiaDatabaseParameterDeltaTc() const;

  double sandiaDatabaseParameterfd() const;

  double sandiaDatabaseParametera() const;

  double sandiaDatabaseParameterb() const;

  double sandiaDatabaseParameterc4() const;

  double sandiaDatabaseParameterc5() const;

  double sandiaDatabaseParameterIx0() const;

  double sandiaDatabaseParameterIxx0() const;

  double sandiaDatabaseParameterc6() const;

  double sandiaDatabaseParameterc7() const;

  //@}
  /** @name Setters */
  //@{

  bool setActiveArea(double activeArea);

  bool setNumberofCellsinSeries(int numberofCellsinSeries);

  bool setNumberofCellsinParallel(int numberofCellsinParallel);

  bool setShortCircuitCurrent(double shortCircuitCurrent);

  bool setOpenCircuitVoltage(double openCircuitVoltage);

  bool setCurrentatMaximumPowerPoint(double currentatMaximumPowerPoint);

  bool setVoltageatMaximumPowerPoint(double voltageatMaximumPowerPoint);

  bool setSandiaDatabaseParameteraIsc(double sandiaDatabaseParameteraIsc);

  bool setSandiaDatabaseParameteraImp(double sandiaDatabaseParameteraImp);

  bool setSandiaDatabaseParameterc0(double sandiaDatabaseParameterc0);

  bool setSandiaDatabaseParameterc1(double sandiaDatabaseParameterc1);

  bool setSandiaDatabaseParameterBVoc0(double sandiaDatabaseParameterBVoc0);

  bool setSandiaDatabaseParametermBVoc(double sandiaDatabaseParametermBVoc);

  bool setSandiaDatabaseParameterBVmp0(double sandiaDatabaseParameterBVmp0);

  bool setSandiaDatabaseParametermBVmp(double sandiaDatabaseParametermBVmp);

  bool setDiodeFactor(double diodeFactor);

  bool setSandiaDatabaseParameterc2(double sandiaDatabaseParameterc2);

  bool setSandiaDatabaseParameterc3(double sandiaDatabaseParameterc3);

  bool setSandiaDatabaseParametera0(double sandiaDatabaseParametera0);

  bool setSandiaDatabaseParametera1(double sandiaDatabaseParametera1);

  bool setSandiaDatabaseParametera2(double sandiaDatabaseParametera2);

  bool setSandiaDatabaseParametera3(double sandiaDatabaseParametera3);

  bool setSandiaDatabaseParametera4(double sandiaDatabaseParametera4);

  bool setSandiaDatabaseParameterb0(double sandiaDatabaseParameterb0);

  bool setSandiaDatabaseParameterb1(double sandiaDatabaseParameterb1);

  bool setSandiaDatabaseParameterb2(double sandiaDatabaseParameterb2);

  bool setSandiaDatabaseParameterb3(double sandiaDatabaseParameterb3);

  bool setSandiaDatabaseParameterb4(double sandiaDatabaseParameterb4);

  bool setSandiaDatabaseParameterb5(double sandiaDatabaseParameterb5);

  bool setSandiaDatabaseParameterDeltaTc(double sandiaDatabaseParameterDeltaTc);

  bool setSandiaDatabaseParameterfd(double sandiaDatabaseParameterfd);

  bool setSandiaDatabaseParametera(double sandiaDatabaseParametera);

  bool setSandiaDatabaseParameterb(double sandiaDatabaseParameterb);

  bool setSandiaDatabaseParameterc4(double sandiaDatabaseParameterc4);

  bool setSandiaDatabaseParameterc5(double sandiaDatabaseParameterc5);

  bool setSandiaDatabaseParameterIx0(double sandiaDatabaseParameterIx0);

  bool setSandiaDatabaseParameterIxx0(double sandiaDatabaseParameterIxx0);

  bool setSandiaDatabaseParameterc6(double sandiaDatabaseParameterc6);

  bool setSandiaDatabaseParameterc7(double sandiaDatabaseParameterc7);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::PhotovoltaicPerformanceSandia_Impl ImplType;

  explicit PhotovoltaicPerformanceSandia(std::shared_ptr<detail::PhotovoltaicPerformanceSandia_Impl> impl);

  friend class detail::PhotovoltaicPerformanceSandia_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.PhotovoltaicPerformanceSandia");
};

/** \relates PhotovoltaicPerformanceSandia*/
typedef boost::optional<PhotovoltaicPerformanceSandia> OptionalPhotovoltaicPerformanceSandia;

/** \relates PhotovoltaicPerformanceSandia*/
typedef std::vector<PhotovoltaicPerformanceSandia> PhotovoltaicPerformanceSandiaVector;

} // model
} // openstudio

#endif // MODEL_PHOTOVOLTAICPERFORMANCESANDIA_HPP
