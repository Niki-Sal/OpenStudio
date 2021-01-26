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

#include "FanComponentModel.hpp"
#include "FanComponentModel_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Model.hpp"

#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Fan_ComponentModel_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  FanComponentModel_Impl::FanComponentModel_Impl(const IdfObject& idfObject,
                                                 Model_Impl* model,
                                                 bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == FanComponentModel::iddObjectType());
  }

  FanComponentModel_Impl::FanComponentModel_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                 Model_Impl* model,
                                                 bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == FanComponentModel::iddObjectType());
  }

  FanComponentModel_Impl::FanComponentModel_Impl(const FanComponentModel_Impl& other,
                                                 Model_Impl* model,
                                                 bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& FanComponentModel_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType FanComponentModel_Impl::iddObjectType() const {
    return FanComponentModel::iddObjectType();
  }

  std::vector<ScheduleTypeKey> FanComponentModel_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    // TODO: Check schedule display names.
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Fan_ComponentModelFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("FanComponentModel","Availability"));
    }
    return result;
  }

  Connection FanComponentModel_Impl::airInletNode() const {
    boost::optional<Connection> value = optionalAirInletNode();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Air Inlet Node attached.");
    }
    return value.get();
  }

  Connection FanComponentModel_Impl::airOutletNode() const {
    boost::optional<Connection> value = optionalAirOutletNode();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Air Outlet Node attached.");
    }
    return value.get();
  }

  Schedule FanComponentModel_Impl::availabilitySchedule() const {
      boost::optional<Schedule> value =
        getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Fan_ComponentModelFields::AvailabilityScheduleName);
      if (!value) {
        // it is an error if we get here, however we don't want to crash
        // so we hook up to global always on schedule
        LOG(Error, "Required availability schedule not set, using 'Always On' schedule");
        value = this->model().alwaysOnDiscreteSchedule();
        OS_ASSERT(value);
        const_cast<FanComponentModel_Impl*>(this)->setAvailabilitySchedule(*value);
        value = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Fan_ComponentModelFields::AvailabilityScheduleName);
      }
      OS_ASSERT(value);
      return value.get();
    }

  boost::optional<double> FanComponentModel_Impl::maximumFlowRate() const {
    return getDouble(OS_Fan_ComponentModelFields::MaximumFlowRate,true);
  }

  bool FanComponentModel_Impl::isMaximumFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Fan_ComponentModelFields::MaximumFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional <double> FanComponentModel_Impl::autosizedMaximumFlowRate() {
    // TODO: check sql
    return getAutosizedValue("Design Size Maximum Flow Rate", "m3/s");
  }

  boost::optional<double> FanComponentModel_Impl::minimumFlowRate() const {
    return getDouble(OS_Fan_ComponentModelFields::MinimumFlowRate,true);
  }

  bool FanComponentModel_Impl::isMinimumFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Fan_ComponentModelFields::MinimumFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional <double> FanComponentModel_Impl::autosizedMinimumFlowRate() {
    // TODO: check sql
    return getAutosizedValue("Design Size Minimum Flow Rate", "m3/s");
  }

  double FanComponentModel_Impl::fanSizingFactor() const {
    boost::optional<double> value = getDouble(OS_Fan_ComponentModelFields::FanSizingFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  double FanComponentModel_Impl::fanWheelDiameter() const {
    boost::optional<double> value = getDouble(OS_Fan_ComponentModelFields::FanWheelDiameter,true);
    OS_ASSERT(value);
    return value.get();
  }

  double FanComponentModel_Impl::fanOutletArea() const {
    boost::optional<double> value = getDouble(OS_Fan_ComponentModelFields::FanOutletArea,true);
    OS_ASSERT(value);
    return value.get();
  }

  double FanComponentModel_Impl::maximumFanStaticEfficiency() const {
    boost::optional<double> value = getDouble(OS_Fan_ComponentModelFields::MaximumFanStaticEfficiency,true);
    OS_ASSERT(value);
    return value.get();
  }

  double FanComponentModel_Impl::eulerNumberatMaximumFanStaticEfficiency() const {
    boost::optional<double> value = getDouble(OS_Fan_ComponentModelFields::EulerNumberatMaximumFanStaticEfficiency,true);
    OS_ASSERT(value);
    return value.get();
  }

  double FanComponentModel_Impl::maximumDimensionlessFanAirflow() const {
    boost::optional<double> value = getDouble(OS_Fan_ComponentModelFields::MaximumDimensionlessFanAirflow,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> FanComponentModel_Impl::motorFanPulleyRatio() const {
    return getDouble(OS_Fan_ComponentModelFields::MotorFanPulleyRatio,true);
  }

  bool FanComponentModel_Impl::isMotorFanPulleyRatioAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Fan_ComponentModelFields::MotorFanPulleyRatio, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional <double> FanComponentModel_Impl::autosizedMotorFanPulleyRatio() {
    return getAutosizedValue("TODO_CHECK_SQL Motor Fan Pulley Ratio", "");
  }

  boost::optional<double> FanComponentModel_Impl::beltMaximumTorque() const {
    return getDouble(OS_Fan_ComponentModelFields::BeltMaximumTorque,true);
  }

  bool FanComponentModel_Impl::isBeltMaximumTorqueAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Fan_ComponentModelFields::BeltMaximumTorque, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional <double> FanComponentModel_Impl::autosizedBeltMaximumTorque() {
    return getAutosizedValue("TODO_CHECK_SQL Belt Maximum Torque", "N-m");
  }

  double FanComponentModel_Impl::beltSizingFactor() const {
    boost::optional<double> value = getDouble(OS_Fan_ComponentModelFields::BeltSizingFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  double FanComponentModel_Impl::beltFractionalTorqueTransition() const {
    boost::optional<double> value = getDouble(OS_Fan_ComponentModelFields::BeltFractionalTorqueTransition,true);
    OS_ASSERT(value);
    return value.get();
  }

  double FanComponentModel_Impl::motorMaximumSpeed() const {
    boost::optional<double> value = getDouble(OS_Fan_ComponentModelFields::MotorMaximumSpeed,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> FanComponentModel_Impl::maximumMotorOutputPower() const {
    return getDouble(OS_Fan_ComponentModelFields::MaximumMotorOutputPower,true);
  }

  bool FanComponentModel_Impl::isMaximumMotorOutputPowerAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Fan_ComponentModelFields::MaximumMotorOutputPower, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional <double> FanComponentModel_Impl::autosizedMaximumMotorOutputPower() {
    return getAutosizedValue("TODO_CHECK_SQL Maximum Motor Output Power", "W");
  }

  double FanComponentModel_Impl::motorSizingFactor() const {
    boost::optional<double> value = getDouble(OS_Fan_ComponentModelFields::MotorSizingFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  double FanComponentModel_Impl::motorInAirstreamFraction() const {
    boost::optional<double> value = getDouble(OS_Fan_ComponentModelFields::MotorInAirstreamFraction,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<std::string> FanComponentModel_Impl::vFDEfficiencyType() const {
    return getString(OS_Fan_ComponentModelFields::VFDEfficiencyType,true);
  }

  boost::optional<double> FanComponentModel_Impl::maximumVFDOutputPower() const {
    return getDouble(OS_Fan_ComponentModelFields::MaximumVFDOutputPower,true);
  }

  bool FanComponentModel_Impl::isMaximumVFDOutputPowerAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Fan_ComponentModelFields::MaximumVFDOutputPower, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional <double> FanComponentModel_Impl::autosizedMaximumVFDOutputPower() {
    return getAutosizedValue("TODO_CHECK_SQL Maximum VFD Output Power", "W");
  }

  double FanComponentModel_Impl::vFDSizingFactor() const {
    boost::optional<double> value = getDouble(OS_Fan_ComponentModelFields::VFDSizingFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  Curve FanComponentModel_Impl::fanPressureRiseCurve() const {
    boost::optional<Curve> value = optionalFanPressureRiseCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Fan Pressure Rise Curve attached.");
    }
    return value.get();
  }

  Curve FanComponentModel_Impl::ductStaticPressureResetCurve() const {
    boost::optional<Curve> value = optionalDuctStaticPressureResetCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Duct Static Pressure Reset Curve attached.");
    }
    return value.get();
  }

  Curve FanComponentModel_Impl::normalizedFanStaticEfficiencyCurveNonStallRegion() const {
    boost::optional<Curve> value = optionalNormalizedFanStaticEfficiencyCurveNonStallRegion();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Normalized Fan Static Efficiency Curve Non Stall Region attached.");
    }
    return value.get();
  }

  Curve FanComponentModel_Impl::normalizedFanStaticEfficiencyCurveStallRegion() const {
    boost::optional<Curve> value = optionalNormalizedFanStaticEfficiencyCurveStallRegion();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Normalized Fan Static Efficiency Curve Stall Region attached.");
    }
    return value.get();
  }

  Curve FanComponentModel_Impl::normalizedDimensionlessAirflowCurveNonStallRegion() const {
    boost::optional<Curve> value = optionalNormalizedDimensionlessAirflowCurveNonStallRegion();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Normalized Dimensionless Airflow Curve Non Stall Region attached.");
    }
    return value.get();
  }

  Curve FanComponentModel_Impl::normalizedDimensionlessAirflowCurveStallRegion() const {
    boost::optional<Curve> value = optionalNormalizedDimensionlessAirflowCurveStallRegion();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Normalized Dimensionless Airflow Curve Stall Region attached.");
    }
    return value.get();
  }

  boost::optional<Curve> FanComponentModel_Impl::maximumBeltEfficiencyCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Fan_ComponentModelFields::MaximumBeltEfficiencyCurveName);
  }

  boost::optional<Curve> FanComponentModel_Impl::normalizedBeltEfficiencyCurveRegion1() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Fan_ComponentModelFields::NormalizedBeltEfficiencyCurveNameRegion1);
  }

  boost::optional<Curve> FanComponentModel_Impl::normalizedBeltEfficiencyCurveRegion2() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Fan_ComponentModelFields::NormalizedBeltEfficiencyCurveNameRegion2);
  }

  boost::optional<Curve> FanComponentModel_Impl::normalizedBeltEfficiencyCurveRegion3() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Fan_ComponentModelFields::NormalizedBeltEfficiencyCurveNameRegion3);
  }

  boost::optional<Curve> FanComponentModel_Impl::maximumMotorEfficiencyCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Fan_ComponentModelFields::MaximumMotorEfficiencyCurveName);
  }

  boost::optional<Curve> FanComponentModel_Impl::normalizedMotorEfficiencyCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Fan_ComponentModelFields::NormalizedMotorEfficiencyCurveName);
  }

  boost::optional<Curve> FanComponentModel_Impl::vFDEfficiencyCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Fan_ComponentModelFields::VFDEfficiencyCurveName);
  }

  std::string FanComponentModel_Impl::endUseSubcategory() const {
    boost::optional<std::string> value = getString(OS_Fan_ComponentModelFields::EndUseSubcategory,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool FanComponentModel_Impl::setAirInletNode(const Connection& connection) {
    bool result = setPointer(OS_Fan_ComponentModelFields::AirInletNodeName, connection.handle());
    return result;
  }

  bool FanComponentModel_Impl::setAirOutletNode(const Connection& connection) {
    bool result = setPointer(OS_Fan_ComponentModelFields::AirOutletNodeName, connection.handle());
    return result;
  }

  bool FanComponentModel_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Fan_ComponentModelFields::AvailabilityScheduleName,
                              "FanComponentModel",
                              "Availability",
                              schedule);
    return result;
  }

  bool FanComponentModel_Impl::setMaximumFlowRate(double maximumFlowRate) {
    bool result = setDouble(OS_Fan_ComponentModelFields::MaximumFlowRate, maximumFlowRate);
    return result;
  }

  void FanComponentModel_Impl::autosizeMaximumFlowRate() {
    bool result = setString(OS_Fan_ComponentModelFields::MaximumFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool FanComponentModel_Impl::setMinimumFlowRate(double minimumFlowRate) {
    bool result = setDouble(OS_Fan_ComponentModelFields::MinimumFlowRate, minimumFlowRate);
    return result;
  }

  void FanComponentModel_Impl::autosizeMinimumFlowRate() {
    bool result = setString(OS_Fan_ComponentModelFields::MinimumFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool FanComponentModel_Impl::setFanSizingFactor(double fanSizingFactor) {
    bool result = setDouble(OS_Fan_ComponentModelFields::FanSizingFactor, fanSizingFactor);
    return result;
  }

  bool FanComponentModel_Impl::setFanWheelDiameter(double fanWheelDiameter) {
    bool result = setDouble(OS_Fan_ComponentModelFields::FanWheelDiameter, fanWheelDiameter);
    return result;
  }

  bool FanComponentModel_Impl::setFanOutletArea(double fanOutletArea) {
    bool result = setDouble(OS_Fan_ComponentModelFields::FanOutletArea, fanOutletArea);
    return result;
  }

  bool FanComponentModel_Impl::setMaximumFanStaticEfficiency(double maximumFanStaticEfficiency) {
    bool result = setDouble(OS_Fan_ComponentModelFields::MaximumFanStaticEfficiency, maximumFanStaticEfficiency);
    return result;
  }

  bool FanComponentModel_Impl::setEulerNumberatMaximumFanStaticEfficiency(double eulerNumberatMaximumFanStaticEfficiency) {
    bool result = setDouble(OS_Fan_ComponentModelFields::EulerNumberatMaximumFanStaticEfficiency, eulerNumberatMaximumFanStaticEfficiency);
    return result;
  }

  bool FanComponentModel_Impl::setMaximumDimensionlessFanAirflow(double maximumDimensionlessFanAirflow) {
    bool result = setDouble(OS_Fan_ComponentModelFields::MaximumDimensionlessFanAirflow, maximumDimensionlessFanAirflow);
    return result;
  }

  bool FanComponentModel_Impl::setMotorFanPulleyRatio(double motorFanPulleyRatio) {
    bool result = setDouble(OS_Fan_ComponentModelFields::MotorFanPulleyRatio, motorFanPulleyRatio);
    return result;
  }

  void FanComponentModel_Impl::autosizeMotorFanPulleyRatio() {
    bool result = setString(OS_Fan_ComponentModelFields::MotorFanPulleyRatio, "autosize");
    OS_ASSERT(result);
  }

  bool FanComponentModel_Impl::setBeltMaximumTorque(double beltMaximumTorque) {
    bool result = setDouble(OS_Fan_ComponentModelFields::BeltMaximumTorque, beltMaximumTorque);
    return result;
  }

  void FanComponentModel_Impl::autosizeBeltMaximumTorque() {
    bool result = setString(OS_Fan_ComponentModelFields::BeltMaximumTorque, "autosize");
    OS_ASSERT(result);
  }

  bool FanComponentModel_Impl::setBeltSizingFactor(double beltSizingFactor) {
    bool result = setDouble(OS_Fan_ComponentModelFields::BeltSizingFactor, beltSizingFactor);
    return result;
  }

  bool FanComponentModel_Impl::setBeltFractionalTorqueTransition(double beltFractionalTorqueTransition) {
    bool result = setDouble(OS_Fan_ComponentModelFields::BeltFractionalTorqueTransition, beltFractionalTorqueTransition);
    return result;
  }

  bool FanComponentModel_Impl::setMotorMaximumSpeed(double motorMaximumSpeed) {
    bool result = setDouble(OS_Fan_ComponentModelFields::MotorMaximumSpeed, motorMaximumSpeed);
    return result;
  }

  bool FanComponentModel_Impl::setMaximumMotorOutputPower(double maximumMotorOutputPower) {
    bool result = setDouble(OS_Fan_ComponentModelFields::MaximumMotorOutputPower, maximumMotorOutputPower);
    return result;
  }

  void FanComponentModel_Impl::autosizeMaximumMotorOutputPower() {
    bool result = setString(OS_Fan_ComponentModelFields::MaximumMotorOutputPower, "autosize");
    OS_ASSERT(result);
  }

  bool FanComponentModel_Impl::setMotorSizingFactor(double motorSizingFactor) {
    bool result = setDouble(OS_Fan_ComponentModelFields::MotorSizingFactor, motorSizingFactor);
    return result;
  }

  bool FanComponentModel_Impl::setMotorInAirstreamFraction(double motorInAirstreamFraction) {
    bool result = setDouble(OS_Fan_ComponentModelFields::MotorInAirstreamFraction, motorInAirstreamFraction);
    return result;
  }

  bool FanComponentModel_Impl::setVFDEfficiencyType(const std::string& vFDEfficiencyType) {
    bool result = setString(OS_Fan_ComponentModelFields::VFDEfficiencyType, vFDEfficiencyType);
    return result;
  }

  void FanComponentModel_Impl::resetVFDEfficiencyType() {
    bool result = setString(OS_Fan_ComponentModelFields::VFDEfficiencyType, "");
    OS_ASSERT(result);
  }

  bool FanComponentModel_Impl::setMaximumVFDOutputPower(double maximumVFDOutputPower) {
    bool result = setDouble(OS_Fan_ComponentModelFields::MaximumVFDOutputPower, maximumVFDOutputPower);
    return result;
  }

  void FanComponentModel_Impl::autosizeMaximumVFDOutputPower() {
    bool result = setString(OS_Fan_ComponentModelFields::MaximumVFDOutputPower, "autosize");
    OS_ASSERT(result);
  }

  bool FanComponentModel_Impl::setVFDSizingFactor(double vFDSizingFactor) {
    bool result = setDouble(OS_Fan_ComponentModelFields::VFDSizingFactor, vFDSizingFactor);
    return result;
  }

  bool FanComponentModel_Impl::setFanPressureRiseCurve(const Curve& bivariateFunctions) {
    bool result = setPointer(OS_Fan_ComponentModelFields::FanPressureRiseCurveName, bivariateFunctions.handle());
    return result;
  }

  bool FanComponentModel_Impl::setDuctStaticPressureResetCurve(const Curve& univariateFunctions) {
    bool result = setPointer(OS_Fan_ComponentModelFields::DuctStaticPressureResetCurveName, univariateFunctions.handle());
    return result;
  }

  bool FanComponentModel_Impl::setNormalizedFanStaticEfficiencyCurveNonStallRegion(const Curve& univariateFunctions) {
    bool result = setPointer(OS_Fan_ComponentModelFields::NormalizedFanStaticEfficiencyCurveNameNonStallRegion, univariateFunctions.handle());
    return result;
  }

  bool FanComponentModel_Impl::setNormalizedFanStaticEfficiencyCurveStallRegion(const Curve& univariateFunctions) {
    bool result = setPointer(OS_Fan_ComponentModelFields::NormalizedFanStaticEfficiencyCurveNameStallRegion, univariateFunctions.handle());
    return result;
  }

  bool FanComponentModel_Impl::setNormalizedDimensionlessAirflowCurveNonStallRegion(const Curve& univariateFunctions) {
    bool result = setPointer(OS_Fan_ComponentModelFields::NormalizedDimensionlessAirflowCurveNameNonStallRegion, univariateFunctions.handle());
    return result;
  }

  bool FanComponentModel_Impl::setNormalizedDimensionlessAirflowCurveStallRegion(const Curve& univariateFunctions) {
    bool result = setPointer(OS_Fan_ComponentModelFields::NormalizedDimensionlessAirflowCurveNameStallRegion, univariateFunctions.handle());
    return result;
  }

  bool FanComponentModel_Impl::setMaximumBeltEfficiencyCurve(const Curve& univariateFunctions) {
    bool result = setPointer(OS_Fan_ComponentModelFields::MaximumBeltEfficiencyCurveName, univariateFunctions.handle());
    return result;
  }

  void FanComponentModel_Impl::resetMaximumBeltEfficiencyCurve() {
    bool result = setString(OS_Fan_ComponentModelFields::MaximumBeltEfficiencyCurveName, "");
    OS_ASSERT(result);
  }

  bool FanComponentModel_Impl::setNormalizedBeltEfficiencyCurveRegion1(const Curve& univariateFunctions) {
    bool result = setPointer(OS_Fan_ComponentModelFields::NormalizedBeltEfficiencyCurveNameRegion1, univariateFunctions.handle());
    return result;
  }

  void FanComponentModel_Impl::resetNormalizedBeltEfficiencyCurveRegion1() {
    bool result = setString(OS_Fan_ComponentModelFields::NormalizedBeltEfficiencyCurveNameRegion1, "");
    OS_ASSERT(result);
  }

  bool FanComponentModel_Impl::setNormalizedBeltEfficiencyCurveRegion2(const Curve& univariateFunctions) {
    bool result = setPointer(OS_Fan_ComponentModelFields::NormalizedBeltEfficiencyCurveNameRegion2, univariateFunctions.handle());
    return result;
  }

  void FanComponentModel_Impl::resetNormalizedBeltEfficiencyCurveRegion2() {
    bool result = setString(OS_Fan_ComponentModelFields::NormalizedBeltEfficiencyCurveNameRegion2, "");
    OS_ASSERT(result);
  }

  bool FanComponentModel_Impl::setNormalizedBeltEfficiencyCurveRegion3(const Curve& univariateFunctions) {
    bool result = setPointer(OS_Fan_ComponentModelFields::NormalizedBeltEfficiencyCurveNameRegion3, univariateFunctions.handle());
    return result;
  }

  void FanComponentModel_Impl::resetNormalizedBeltEfficiencyCurveRegion3() {
    bool result = setString(OS_Fan_ComponentModelFields::NormalizedBeltEfficiencyCurveNameRegion3, "");
    OS_ASSERT(result);
  }

  bool FanComponentModel_Impl::setMaximumMotorEfficiencyCurve(const Curve& univariateFunctions) {
    bool result = setPointer(OS_Fan_ComponentModelFields::MaximumMotorEfficiencyCurveName, univariateFunctions.handle());
    return result;
  }

  void FanComponentModel_Impl::resetMaximumMotorEfficiencyCurve() {
    bool result = setString(OS_Fan_ComponentModelFields::MaximumMotorEfficiencyCurveName, "");
    OS_ASSERT(result);
  }

  bool FanComponentModel_Impl::setNormalizedMotorEfficiencyCurve(const Curve& univariateFunctions) {
    bool result = setPointer(OS_Fan_ComponentModelFields::NormalizedMotorEfficiencyCurveName, univariateFunctions.handle());
    return result;
  }

  void FanComponentModel_Impl::resetNormalizedMotorEfficiencyCurve() {
    bool result = setString(OS_Fan_ComponentModelFields::NormalizedMotorEfficiencyCurveName, "");
    OS_ASSERT(result);
  }

  bool FanComponentModel_Impl::setVFDEfficiencyCurve(const Curve& univariateFunctions) {
    bool result = setPointer(OS_Fan_ComponentModelFields::VFDEfficiencyCurveName, univariateFunctions.handle());
    return result;
  }

  void FanComponentModel_Impl::resetVFDEfficiencyCurve() {
    bool result = setString(OS_Fan_ComponentModelFields::VFDEfficiencyCurveName, "");
    OS_ASSERT(result);
  }

  bool FanComponentModel_Impl::setEndUseSubcategory(const std::string& endUseSubcategory) {
    bool result = setString(OS_Fan_ComponentModelFields::EndUseSubcategory, endUseSubcategory);
    OS_ASSERT(result);
    return result;
  }

  void FanComponentModel_Impl::autosize() {
    autosizedMaximumFlowRate();
    autosizedMinimumFlowRate();
    autosizeMotorFanPulleyRatio();
    autosizeBeltMaximumTorque();
    autosizeMaximumMotorOutputPower();
    autosizeMaximumVFDOutputPower();
  }

  void FanComponentModel_Impl::applySizingValues() {
    boost::optional<double> val;

    val = autosizedMaximumFlowRate();
    if (val) {
      setMaximumFlowRate(val.get());
    }

    val = autosizedMinimumFlowRate();
    if (val) {
      setMinimumFlowRate(val.get());
    }

    val = autosizedMotorFanPulleyRatio();
    if (val) {
      setMotorFanPulleyRatio(val.get());
    }

    val = autosizedBeltMaximumTorque();
    if (val) {
      setBeltMaximumTorque(val.get());
    }

    val = autosizedMaximumMotorOutputPower();
    if (val) {
      setMaximumMotorOutputPower(val.get());
    }

    val = autosizedMaximumVFDOutputPower();
    if (val) {
      setMaximumVFDOutputPower(val.get());
    }

  }

  boost::optional<Curve> FanComponentModel_Impl::optionalFanPressureRiseCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Fan_ComponentModelFields::FanPressureRiseCurveName);
  }

  boost::optional<Curve> FanComponentModel_Impl::optionalDuctStaticPressureResetCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Fan_ComponentModelFields::DuctStaticPressureResetCurveName);
  }

  boost::optional<Curve> FanComponentModel_Impl::optionalNormalizedFanStaticEfficiencyCurveNonStallRegion() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Fan_ComponentModelFields::NormalizedFanStaticEfficiencyCurveNameNonStallRegion);
  }

  boost::optional<Curve> FanComponentModel_Impl::optionalNormalizedFanStaticEfficiencyCurveStallRegion() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Fan_ComponentModelFields::NormalizedFanStaticEfficiencyCurveNameStallRegion);
  }

  boost::optional<Curve> FanComponentModel_Impl::optionalNormalizedDimensionlessAirflowCurveNonStallRegion() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Fan_ComponentModelFields::NormalizedDimensionlessAirflowCurveNameNonStallRegion);
  }

  boost::optional<Curve> FanComponentModel_Impl::optionalNormalizedDimensionlessAirflowCurveStallRegion() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Fan_ComponentModelFields::NormalizedDimensionlessAirflowCurveNameStallRegion);
  }

} // detail

FanComponentModel::FanComponentModel(const Model& model)
  : StraightComponent(FanComponentModel::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::FanComponentModel_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  //     OS_Fan_ComponentModelFields::AirInletNodeName
  //     OS_Fan_ComponentModelFields::AirOutletNodeName
  //     OS_Fan_ComponentModelFields::AvailabilityScheduleName
  //     OS_Fan_ComponentModelFields::FanPressureRiseCurveName
  //     OS_Fan_ComponentModelFields::DuctStaticPressureResetCurveName
  //     OS_Fan_ComponentModelFields::NormalizedFanStaticEfficiencyCurveNameNonStallRegion
  //     OS_Fan_ComponentModelFields::NormalizedFanStaticEfficiencyCurveNameStallRegion
  //     OS_Fan_ComponentModelFields::NormalizedDimensionlessAirflowCurveNameNonStallRegion
  //     OS_Fan_ComponentModelFields::NormalizedDimensionlessAirflowCurveNameStallRegion
  bool ok = true;
  // ok = setAirInletNode();
  OS_ASSERT(ok);
  // ok = setAirOutletNode();
  OS_ASSERT(ok);

  auto availabilitySchedule = model.alwaysOnDiscreteSchedule();
  ok = setAvailabilitySchedule(availabilitySchedule);
  OS_ASSERT(ok);

  autosizeMaximumFlowRate();
  autosizeMinimumFlowRate();

  // E+ IDD default
  autosizeMaximumFlowRate();
  autosizeMinimumFlowRate();
  ok = setFanSizingFactor(1.0);
  OS_ASSERT(ok);
  ok = setMotorFanPulleyRatio(1.0); // TODO: or autosizeMotorFanPulleyRatio()
  OS_ASSERT(ok);
  ok = setBeltSizingFactor(1.0);
  OS_ASSERT(ok);
  ok = setBeltFractionalTorqueTransition(0.167);
  OS_ASSERT(ok);
  ok = setMotorSizingFactor(1.0);
  OS_ASSERT(ok);
  ok = setMotorInAirstreamFraction(1.0);
  OS_ASSERT(ok);
  ok = setVFDSizingFactor(1.0);
  OS_ASSERT(ok);
  ok = setEndUseSubcategory("General");
  OS_ASSERT(ok);

  // TODO: Provide default for these

  // ok = setFanWheelDiameter();
  OS_ASSERT(ok);
  // ok = setFanOutletArea();
  OS_ASSERT(ok);
  // ok = setMaximumFanStaticEfficiency();
  OS_ASSERT(ok);
  // ok = setEulerNumberatMaximumFanStaticEfficiency();
  OS_ASSERT(ok);
  // ok = setMaximumDimensionlessFanAirflow();
  OS_ASSERT(ok);
  // ok = setBeltMaximumTorque();
  OS_ASSERT(ok);
  // ok = setMotorMaximumSpeed();
  OS_ASSERT(ok);
  // ok = setMaximumMotorOutputPower();
  OS_ASSERT(ok);
  // ok = setMaximumVFDOutputPower();
  OS_ASSERT(ok);
  // ok = setFanPressureRiseCurve();
  OS_ASSERT(ok);
  // ok = setDuctStaticPressureResetCurve();
  OS_ASSERT(ok);
  // ok = setNormalizedFanStaticEfficiencyCurveNonStallRegion();
  OS_ASSERT(ok);
  // ok = setNormalizedFanStaticEfficiencyCurveStallRegion();
  OS_ASSERT(ok);
  // ok = setNormalizedDimensionlessAirflowCurveNonStallRegion();
  OS_ASSERT(ok);
  // ok = setNormalizedDimensionlessAirflowCurveStallRegion();
  OS_ASSERT(ok);

}

IddObjectType FanComponentModel::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Fan_ComponentModel);
}

std::vector<std::string> FanComponentModel::vFDEfficiencyTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Fan_ComponentModelFields::VFDEfficiencyType);
}

Schedule FanComponentModel::availabilitySchedule() const {
  return getImpl<detail::FanComponentModel_Impl>()->availabilitySchedule();
}

boost::optional<double> FanComponentModel::maximumFlowRate() const {
  return getImpl<detail::FanComponentModel_Impl>()->maximumFlowRate();
}

bool FanComponentModel::isMaximumFlowRateAutosized() const {
  return getImpl<detail::FanComponentModel_Impl>()->isMaximumFlowRateAutosized();
}

boost::optional <double> FanComponentModel::autosizedMaximumFlowRate() {
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->autosizedMaximumFlowRate();
}

boost::optional<double> FanComponentModel::minimumFlowRate() const {
  return getImpl<detail::FanComponentModel_Impl>()->minimumFlowRate();
}

bool FanComponentModel::isMinimumFlowRateAutosized() const {
  return getImpl<detail::FanComponentModel_Impl>()->isMinimumFlowRateAutosized();
}

boost::optional <double> FanComponentModel::autosizedMinimumFlowRate() {
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->autosizedMinimumFlowRate();
}

double FanComponentModel::fanSizingFactor() const {
  return getImpl<detail::FanComponentModel_Impl>()->fanSizingFactor();
}

double FanComponentModel::fanWheelDiameter() const {
  return getImpl<detail::FanComponentModel_Impl>()->fanWheelDiameter();
}

double FanComponentModel::fanOutletArea() const {
  return getImpl<detail::FanComponentModel_Impl>()->fanOutletArea();
}

double FanComponentModel::maximumFanStaticEfficiency() const {
  return getImpl<detail::FanComponentModel_Impl>()->maximumFanStaticEfficiency();
}

double FanComponentModel::eulerNumberatMaximumFanStaticEfficiency() const {
  return getImpl<detail::FanComponentModel_Impl>()->eulerNumberatMaximumFanStaticEfficiency();
}

double FanComponentModel::maximumDimensionlessFanAirflow() const {
  return getImpl<detail::FanComponentModel_Impl>()->maximumDimensionlessFanAirflow();
}

boost::optional<double> FanComponentModel::motorFanPulleyRatio() const {
  return getImpl<detail::FanComponentModel_Impl>()->motorFanPulleyRatio();
}

bool FanComponentModel::isMotorFanPulleyRatioAutosized() const {
  return getImpl<detail::FanComponentModel_Impl>()->isMotorFanPulleyRatioAutosized();
}

boost::optional <double> FanComponentModel::autosizedMotorFanPulleyRatio() {
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->autosizedMotorFanPulleyRatio();
}

boost::optional<double> FanComponentModel::beltMaximumTorque() const {
  return getImpl<detail::FanComponentModel_Impl>()->beltMaximumTorque();
}

bool FanComponentModel::isBeltMaximumTorqueAutosized() const {
  return getImpl<detail::FanComponentModel_Impl>()->isBeltMaximumTorqueAutosized();
}

boost::optional <double> FanComponentModel::autosizedBeltMaximumTorque() {
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->autosizedBeltMaximumTorque();
}

double FanComponentModel::beltSizingFactor() const {
  return getImpl<detail::FanComponentModel_Impl>()->beltSizingFactor();
}

double FanComponentModel::beltFractionalTorqueTransition() const {
  return getImpl<detail::FanComponentModel_Impl>()->beltFractionalTorqueTransition();
}

double FanComponentModel::motorMaximumSpeed() const {
  return getImpl<detail::FanComponentModel_Impl>()->motorMaximumSpeed();
}

boost::optional<double> FanComponentModel::maximumMotorOutputPower() const {
  return getImpl<detail::FanComponentModel_Impl>()->maximumMotorOutputPower();
}

bool FanComponentModel::isMaximumMotorOutputPowerAutosized() const {
  return getImpl<detail::FanComponentModel_Impl>()->isMaximumMotorOutputPowerAutosized();
}

boost::optional <double> FanComponentModel::autosizedMaximumMotorOutputPower() {
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->autosizedMaximumMotorOutputPower();
}

double FanComponentModel::motorSizingFactor() const {
  return getImpl<detail::FanComponentModel_Impl>()->motorSizingFactor();
}

double FanComponentModel::motorInAirstreamFraction() const {
  return getImpl<detail::FanComponentModel_Impl>()->motorInAirstreamFraction();
}

boost::optional<std::string> FanComponentModel::vFDEfficiencyType() const {
  return getImpl<detail::FanComponentModel_Impl>()->vFDEfficiencyType();
}

boost::optional<double> FanComponentModel::maximumVFDOutputPower() const {
  return getImpl<detail::FanComponentModel_Impl>()->maximumVFDOutputPower();
}

bool FanComponentModel::isMaximumVFDOutputPowerAutosized() const {
  return getImpl<detail::FanComponentModel_Impl>()->isMaximumVFDOutputPowerAutosized();
}

boost::optional <double> FanComponentModel::autosizedMaximumVFDOutputPower() {
  return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->autosizedMaximumVFDOutputPower();
}

double FanComponentModel::vFDSizingFactor() const {
  return getImpl<detail::FanComponentModel_Impl>()->vFDSizingFactor();
}

Curve FanComponentModel::fanPressureRiseCurve() const {
  return getImpl<detail::FanComponentModel_Impl>()->fanPressureRiseCurve();
}

Curve FanComponentModel::ductStaticPressureResetCurve() const {
  return getImpl<detail::FanComponentModel_Impl>()->ductStaticPressureResetCurve();
}

Curve FanComponentModel::normalizedFanStaticEfficiencyCurveNonStallRegion() const {
  return getImpl<detail::FanComponentModel_Impl>()->normalizedFanStaticEfficiencyCurveNonStallRegion();
}

Curve FanComponentModel::normalizedFanStaticEfficiencyCurveStallRegion() const {
  return getImpl<detail::FanComponentModel_Impl>()->normalizedFanStaticEfficiencyCurveStallRegion();
}

Curve FanComponentModel::normalizedDimensionlessAirflowCurveNonStallRegion() const {
  return getImpl<detail::FanComponentModel_Impl>()->normalizedDimensionlessAirflowCurveNonStallRegion();
}

Curve FanComponentModel::normalizedDimensionlessAirflowCurveStallRegion() const {
  return getImpl<detail::FanComponentModel_Impl>()->normalizedDimensionlessAirflowCurveStallRegion();
}

boost::optional<Curve> FanComponentModel::maximumBeltEfficiencyCurve() const {
  return getImpl<detail::FanComponentModel_Impl>()->maximumBeltEfficiencyCurve();
}

boost::optional<Curve> FanComponentModel::normalizedBeltEfficiencyCurveRegion1() const {
  return getImpl<detail::FanComponentModel_Impl>()->normalizedBeltEfficiencyCurveRegion1();
}

boost::optional<Curve> FanComponentModel::normalizedBeltEfficiencyCurveRegion2() const {
  return getImpl<detail::FanComponentModel_Impl>()->normalizedBeltEfficiencyCurveRegion2();
}

boost::optional<Curve> FanComponentModel::normalizedBeltEfficiencyCurveRegion3() const {
  return getImpl<detail::FanComponentModel_Impl>()->normalizedBeltEfficiencyCurveRegion3();
}

boost::optional<Curve> FanComponentModel::maximumMotorEfficiencyCurve() const {
  return getImpl<detail::FanComponentModel_Impl>()->maximumMotorEfficiencyCurve();
}

boost::optional<Curve> FanComponentModel::normalizedMotorEfficiencyCurve() const {
  return getImpl<detail::FanComponentModel_Impl>()->normalizedMotorEfficiencyCurve();
}

boost::optional<Curve> FanComponentModel::vFDEfficiencyCurve() const {
  return getImpl<detail::FanComponentModel_Impl>()->vFDEfficiencyCurve();
}

std::string FanComponentModel::endUseSubcategory() const {
  return getImpl<detail::FanComponentModel_Impl>()->endUseSubcategory();
}

bool FanComponentModel::setAirInletNode(const Connection& connection) {
  return getImpl<detail::FanComponentModel_Impl>()->setAirInletNode(connection);
}

bool FanComponentModel::setAirOutletNode(const Connection& connection) {
  return getImpl<detail::FanComponentModel_Impl>()->setAirOutletNode(connection);
}

bool FanComponentModel::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::FanComponentModel_Impl>()->setAvailabilitySchedule(schedule);
}

bool FanComponentModel::setMaximumFlowRate(double maximumFlowRate) {
  return getImpl<detail::FanComponentModel_Impl>()->setMaximumFlowRate(maximumFlowRate);
}

void FanComponentModel::autosizeMaximumFlowRate() {
  getImpl<detail::FanComponentModel_Impl>()->autosizeMaximumFlowRate();
}

bool FanComponentModel::setMinimumFlowRate(double minimumFlowRate) {
  return getImpl<detail::FanComponentModel_Impl>()->setMinimumFlowRate(minimumFlowRate);
}

void FanComponentModel::autosizeMinimumFlowRate() {
  getImpl<detail::FanComponentModel_Impl>()->autosizeMinimumFlowRate();
}

bool FanComponentModel::setFanSizingFactor(double fanSizingFactor) {
  return getImpl<detail::FanComponentModel_Impl>()->setFanSizingFactor(fanSizingFactor);
}

bool FanComponentModel::setFanWheelDiameter(double fanWheelDiameter) {
  return getImpl<detail::FanComponentModel_Impl>()->setFanWheelDiameter(fanWheelDiameter);
}

bool FanComponentModel::setFanOutletArea(double fanOutletArea) {
  return getImpl<detail::FanComponentModel_Impl>()->setFanOutletArea(fanOutletArea);
}

bool FanComponentModel::setMaximumFanStaticEfficiency(double maximumFanStaticEfficiency) {
  return getImpl<detail::FanComponentModel_Impl>()->setMaximumFanStaticEfficiency(maximumFanStaticEfficiency);
}

bool FanComponentModel::setEulerNumberatMaximumFanStaticEfficiency(double eulerNumberatMaximumFanStaticEfficiency) {
  return getImpl<detail::FanComponentModel_Impl>()->setEulerNumberatMaximumFanStaticEfficiency(eulerNumberatMaximumFanStaticEfficiency);
}

bool FanComponentModel::setMaximumDimensionlessFanAirflow(double maximumDimensionlessFanAirflow) {
  return getImpl<detail::FanComponentModel_Impl>()->setMaximumDimensionlessFanAirflow(maximumDimensionlessFanAirflow);
}

bool FanComponentModel::setMotorFanPulleyRatio(double motorFanPulleyRatio) {
  return getImpl<detail::FanComponentModel_Impl>()->setMotorFanPulleyRatio(motorFanPulleyRatio);
}

void FanComponentModel::autosizeMotorFanPulleyRatio() {
  getImpl<detail::FanComponentModel_Impl>()->autosizeMotorFanPulleyRatio();
}

bool FanComponentModel::setBeltMaximumTorque(double beltMaximumTorque) {
  return getImpl<detail::FanComponentModel_Impl>()->setBeltMaximumTorque(beltMaximumTorque);
}

void FanComponentModel::autosizeBeltMaximumTorque() {
  getImpl<detail::FanComponentModel_Impl>()->autosizeBeltMaximumTorque();
}

bool FanComponentModel::setBeltSizingFactor(double beltSizingFactor) {
  return getImpl<detail::FanComponentModel_Impl>()->setBeltSizingFactor(beltSizingFactor);
}

bool FanComponentModel::setBeltFractionalTorqueTransition(double beltFractionalTorqueTransition) {
  return getImpl<detail::FanComponentModel_Impl>()->setBeltFractionalTorqueTransition(beltFractionalTorqueTransition);
}

bool FanComponentModel::setMotorMaximumSpeed(double motorMaximumSpeed) {
  return getImpl<detail::FanComponentModel_Impl>()->setMotorMaximumSpeed(motorMaximumSpeed);
}

bool FanComponentModel::setMaximumMotorOutputPower(double maximumMotorOutputPower) {
  return getImpl<detail::FanComponentModel_Impl>()->setMaximumMotorOutputPower(maximumMotorOutputPower);
}

void FanComponentModel::autosizeMaximumMotorOutputPower() {
  getImpl<detail::FanComponentModel_Impl>()->autosizeMaximumMotorOutputPower();
}

bool FanComponentModel::setMotorSizingFactor(double motorSizingFactor) {
  return getImpl<detail::FanComponentModel_Impl>()->setMotorSizingFactor(motorSizingFactor);
}

bool FanComponentModel::setMotorInAirstreamFraction(double motorInAirstreamFraction) {
  return getImpl<detail::FanComponentModel_Impl>()->setMotorInAirstreamFraction(motorInAirstreamFraction);
}

bool FanComponentModel::setVFDEfficiencyType(const std::string& vFDEfficiencyType) {
  return getImpl<detail::FanComponentModel_Impl>()->setVFDEfficiencyType(vFDEfficiencyType);
}

void FanComponentModel::resetVFDEfficiencyType() {
  getImpl<detail::FanComponentModel_Impl>()->resetVFDEfficiencyType();
}

bool FanComponentModel::setMaximumVFDOutputPower(double maximumVFDOutputPower) {
  return getImpl<detail::FanComponentModel_Impl>()->setMaximumVFDOutputPower(maximumVFDOutputPower);
}

void FanComponentModel::autosizeMaximumVFDOutputPower() {
  getImpl<detail::FanComponentModel_Impl>()->autosizeMaximumVFDOutputPower();
}

bool FanComponentModel::setVFDSizingFactor(double vFDSizingFactor) {
  return getImpl<detail::FanComponentModel_Impl>()->setVFDSizingFactor(vFDSizingFactor);
}

bool FanComponentModel::setFanPressureRiseCurve(const Curve& bivariateFunctions) {
  return getImpl<detail::FanComponentModel_Impl>()->setFanPressureRiseCurve(bivariateFunctions);
}

bool FanComponentModel::setDuctStaticPressureResetCurve(const Curve& univariateFunctions) {
  return getImpl<detail::FanComponentModel_Impl>()->setDuctStaticPressureResetCurve(univariateFunctions);
}

bool FanComponentModel::setNormalizedFanStaticEfficiencyCurveNonStallRegion(const Curve& univariateFunctions) {
  return getImpl<detail::FanComponentModel_Impl>()->setNormalizedFanStaticEfficiencyCurveNonStallRegion(univariateFunctions);
}

bool FanComponentModel::setNormalizedFanStaticEfficiencyCurveStallRegion(const Curve& univariateFunctions) {
  return getImpl<detail::FanComponentModel_Impl>()->setNormalizedFanStaticEfficiencyCurveStallRegion(univariateFunctions);
}

bool FanComponentModel::setNormalizedDimensionlessAirflowCurveNonStallRegion(const Curve& univariateFunctions) {
  return getImpl<detail::FanComponentModel_Impl>()->setNormalizedDimensionlessAirflowCurveNonStallRegion(univariateFunctions);
}

bool FanComponentModel::setNormalizedDimensionlessAirflowCurveStallRegion(const Curve& univariateFunctions) {
  return getImpl<detail::FanComponentModel_Impl>()->setNormalizedDimensionlessAirflowCurveStallRegion(univariateFunctions);
}

bool FanComponentModel::setMaximumBeltEfficiencyCurve(const Curve& univariateFunctions) {
  return getImpl<detail::FanComponentModel_Impl>()->setMaximumBeltEfficiencyCurve(univariateFunctions);
}

void FanComponentModel::resetMaximumBeltEfficiencyCurve() {
  getImpl<detail::FanComponentModel_Impl>()->resetMaximumBeltEfficiencyCurve();
}

bool FanComponentModel::setNormalizedBeltEfficiencyCurveRegion1(const Curve& univariateFunctions) {
  return getImpl<detail::FanComponentModel_Impl>()->setNormalizedBeltEfficiencyCurveRegion1(univariateFunctions);
}

void FanComponentModel::resetNormalizedBeltEfficiencyCurveRegion1() {
  getImpl<detail::FanComponentModel_Impl>()->resetNormalizedBeltEfficiencyCurveRegion1();
}

bool FanComponentModel::setNormalizedBeltEfficiencyCurveRegion2(const Curve& univariateFunctions) {
  return getImpl<detail::FanComponentModel_Impl>()->setNormalizedBeltEfficiencyCurveRegion2(univariateFunctions);
}

void FanComponentModel::resetNormalizedBeltEfficiencyCurveRegion2() {
  getImpl<detail::FanComponentModel_Impl>()->resetNormalizedBeltEfficiencyCurveRegion2();
}

bool FanComponentModel::setNormalizedBeltEfficiencyCurveRegion3(const Curve& univariateFunctions) {
  return getImpl<detail::FanComponentModel_Impl>()->setNormalizedBeltEfficiencyCurveRegion3(univariateFunctions);
}

void FanComponentModel::resetNormalizedBeltEfficiencyCurveRegion3() {
  getImpl<detail::FanComponentModel_Impl>()->resetNormalizedBeltEfficiencyCurveRegion3();
}

bool FanComponentModel::setMaximumMotorEfficiencyCurve(const Curve& univariateFunctions) {
  return getImpl<detail::FanComponentModel_Impl>()->setMaximumMotorEfficiencyCurve(univariateFunctions);
}

void FanComponentModel::resetMaximumMotorEfficiencyCurve() {
  getImpl<detail::FanComponentModel_Impl>()->resetMaximumMotorEfficiencyCurve();
}

bool FanComponentModel::setNormalizedMotorEfficiencyCurve(const Curve& univariateFunctions) {
  return getImpl<detail::FanComponentModel_Impl>()->setNormalizedMotorEfficiencyCurve(univariateFunctions);
}

void FanComponentModel::resetNormalizedMotorEfficiencyCurve() {
  getImpl<detail::FanComponentModel_Impl>()->resetNormalizedMotorEfficiencyCurve();
}

bool FanComponentModel::setVFDEfficiencyCurve(const Curve& univariateFunctions) {
  return getImpl<detail::FanComponentModel_Impl>()->setVFDEfficiencyCurve(univariateFunctions);
}

void FanComponentModel::resetVFDEfficiencyCurve() {
  getImpl<detail::FanComponentModel_Impl>()->resetVFDEfficiencyCurve();
}

bool FanComponentModel::setEndUseSubcategory(const std::string& endUseSubcategory) {
  return getImpl<detail::FanComponentModel_Impl>()->setEndUseSubcategory(endUseSubcategory);
}

/// @cond
FanComponentModel::FanComponentModel(std::shared_ptr<detail::FanComponentModel_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio
