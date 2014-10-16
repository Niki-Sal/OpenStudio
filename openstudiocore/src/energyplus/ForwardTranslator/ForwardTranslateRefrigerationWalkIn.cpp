/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/RefrigerationWalkIn.hpp"
#include "../../model/RefrigerationWalkIn_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/RefrigerationWalkInZoneBoundary.hpp"
#include "../../utilities/time/Time.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/Refrigeration_WalkIn_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateRefrigerationWalkIn( RefrigerationWalkIn & modelObject )
{
  OptionalModelObject temp;
  boost::optional<std::string> s;
  boost::optional<double> d;

// Name
  IdfObject refrigerationWalkIn = createRegisterAndNameIdfObject(openstudio::IddObjectType::Refrigeration_WalkIn,
                                                          modelObject);

//AvailabilityScheduleName
  boost::optional<Schedule> availabilitySchedule = modelObject.availabilitySchedule();

  if( availabilitySchedule )
  {
    boost::optional<IdfObject> _availabilitySchedule = translateAndMapModelObject(availabilitySchedule.get());

    if( _availabilitySchedule && _availabilitySchedule->name() )
    {
      refrigerationWalkIn.setString(Refrigeration_WalkInFields::AvailabilityScheduleName,_availabilitySchedule->name().get());
    }
  }

//RatedCoilCoolingCapacity
  d = modelObject.ratedCoilCoolingCapacity();
  if (d) {
    refrigerationWalkIn.setDouble(Refrigeration_WalkInFields::RatedCoilCoolingCapacity,d.get());
  }

//OperatingTemperature
  d = modelObject.operatingTemperature();
  if (d) {
    refrigerationWalkIn.setDouble(Refrigeration_WalkInFields::OperatingTemperature,d.get());
  }

//RatedCoolingSourceTemperature
  d = modelObject.ratedCoolingSourceTemperature();
  if (d) {
    refrigerationWalkIn.setDouble(Refrigeration_WalkInFields::RatedCoolingSourceTemperature,d.get());
  }

//RatedTotalHeatingPower
  d = modelObject.ratedTotalHeatingPower();
  if (d) {
    refrigerationWalkIn.setDouble(Refrigeration_WalkInFields::RatedTotalHeatingPower,d.get());
  }

//HeatingPowerScheduleName
  boost::optional<Schedule> heatingPowerSchedule = modelObject.heatingPowerSchedule();

  if( heatingPowerSchedule )
  {
    boost::optional<IdfObject> _heatingPowerSchedule = translateAndMapModelObject(heatingPowerSchedule.get());

    if( _heatingPowerSchedule && _heatingPowerSchedule->name() )
    {
      refrigerationWalkIn.setString(Refrigeration_WalkInFields::HeatingPowerScheduleName,_heatingPowerSchedule->name().get());
    }
  }

//RatedCoolingCoilFanPower
  d = modelObject.ratedCoolingCoilFanPower();
  if (d) {
    refrigerationWalkIn.setDouble(Refrigeration_WalkInFields::RatedCoolingCoilFanPower,d.get());
  }

//RatedCirculationFanPower
  d = modelObject.ratedCirculationFanPower();
  if (d) {
    refrigerationWalkIn.setDouble(Refrigeration_WalkInFields::RatedCirculationFanPower,d.get());
  }

//RatedTotalLightingPower
  d = modelObject.ratedTotalLightingPower();
  if (d) {
    refrigerationWalkIn.setDouble(Refrigeration_WalkInFields::RatedTotalLightingPower,d.get());
  }

//LightingScheduleName
  boost::optional<Schedule> lightingSchedule = modelObject.lightingSchedule();

  if( lightingSchedule )
  {
    boost::optional<IdfObject> _lightingSchedule = translateAndMapModelObject(lightingSchedule.get());

    if( _lightingSchedule && _lightingSchedule->name() )
    {
      refrigerationWalkIn.setString(Refrigeration_WalkInFields::LightingScheduleName,_lightingSchedule->name().get());
    }
  }

//DefrostType
  s = modelObject.defrostType();
  if (s) {
    refrigerationWalkIn.setString(Refrigeration_WalkInFields::DefrostType,s.get());
  }

//DefrostControlType
  s = modelObject.defrostControlType();
  if (s) {
    refrigerationWalkIn.setString(Refrigeration_WalkInFields::DefrostControlType,s.get());
  }

//DefrostCycleParameters
  boost::optional<int> durationofDefrostCycle = modelObject.durationofDefrostCycle();
  boost::optional<int> dripDownTime = modelObject.dripDownTime();
  std::vector<openstudio::Time> defrostStartTimes = modelObject.getImpl<model::detail::RefrigerationWalkIn_Impl>()->defrostStartTimes();

  if( durationofDefrostCycle && dripDownTime && !defrostStartTimes.empty() ) {
    int defrostTimeHour = *durationofDefrostCycle / 60;
    int defrostTimeMin = *durationofDefrostCycle % 60;
    int dripDownTimeHour = *dripDownTime / 60;
    int dripDownTimeMin = *dripDownTime % 60;

    std::vector< std::pair<openstudio::Time, double> > defrostDefaultDay;
    std::vector< std::pair<openstudio::Time, double> > dripDownDefaultDay;
    for( std::vector<openstudio::Time>::iterator _defrostStartTime = defrostStartTimes.begin();
       _defrostStartTime != defrostStartTimes.end();
       ++_defrostStartTime )
    {
      defrostDefaultDay.push_back(std::make_pair(*_defrostStartTime, 0)); // defrost off
      openstudio::Time defrostEndTime(0, _defrostStartTime->hours() + defrostTimeHour, _defrostStartTime->minutes() + defrostTimeMin);
      defrostDefaultDay.push_back(std::make_pair(defrostEndTime, 1)); // defrost on

      dripDownDefaultDay.push_back(std::make_pair(*_defrostStartTime, 0)); // drip down off
      openstudio::Time dripDownEndTime(0, _defrostStartTime->hours() + defrostTimeHour + dripDownTimeHour, _defrostStartTime->minutes() + defrostTimeMin + dripDownTimeMin);
      dripDownDefaultDay.push_back(std::make_pair(dripDownEndTime, 1)); // drip down on
    }

    if( (defrostStartTimes.front().hours() != 0 && defrostStartTimes.front().minutes() != 0) || defrostStartTimes.back().hours() < 24) {
      openstudio::Time defrostDayEnd(0, 24, 0);
      defrostDefaultDay.push_back(std::make_pair(defrostDayEnd, 0)); // defrost off
      dripDownDefaultDay.push_back(std::make_pair(defrostDayEnd, 0)); // drip down off
    }

    //DefrostScheduleName
    std::string defrostName(modelObject.name().get() + " Defrost Schedule");
    boost::optional<IdfObject> defrostSchedule = this->createSimpleSchedule(defrostName, defrostDefaultDay);
    if( defrostSchedule ) {
      refrigerationWalkIn.setString(Refrigeration_WalkInFields::DefrostScheduleName, defrostName);
    }

    //DefrostDripDownScheduleName
    std::string dripDownName(modelObject.name().get() + " Defrost Drip Down Schedule");
    boost::optional<IdfObject> defrostDripDownSchedule = this->createSimpleSchedule(dripDownName, dripDownDefaultDay);
    if( defrostDripDownSchedule ) {
      refrigerationWalkIn.setString(Refrigeration_WalkInFields::DefrostDripDownScheduleName, dripDownName);
    }
  } else {
  //DefrostScheduleName
    boost::optional<Schedule> defrostSchedule = modelObject.defrostSchedule();

    if( defrostSchedule )
    {
      boost::optional<IdfObject> _defrostSchedule = translateAndMapModelObject(defrostSchedule.get());

      if( _defrostSchedule && _defrostSchedule->name() )
      {
        refrigerationWalkIn.setString(Refrigeration_WalkInFields::DefrostScheduleName,_defrostSchedule->name().get());
      }
    }

  //DefrostDripDownScheduleName
    boost::optional<Schedule> defrostDripDownSchedule = modelObject.defrostDripDownSchedule();

    if( defrostDripDownSchedule )
    {
      boost::optional<IdfObject> _defrostDripDownSchedule = translateAndMapModelObject(defrostDripDownSchedule.get());

      if( _defrostDripDownSchedule && _defrostDripDownSchedule->name() )
      {
        refrigerationWalkIn.setString(Refrigeration_WalkInFields::DefrostDripDownScheduleName,_defrostDripDownSchedule->name().get());
      }
    }
  }

//DefrostPower
  d = modelObject.defrostPower();
  if (d) {
    refrigerationWalkIn.setDouble(Refrigeration_WalkInFields::DefrostPower,d.get());
  }

//TemperatureTerminationDefrostFractiontoIce
  d = modelObject.temperatureTerminationDefrostFractiontoIce();
  if (d) {
    refrigerationWalkIn.setDouble(Refrigeration_WalkInFields::TemperatureTerminationDefrostFractiontoIce,d.get());
  }

//RestockingScheduleName
  boost::optional<Schedule> restockingSchedule = modelObject.restockingSchedule();

  if( restockingSchedule )
  {
    boost::optional<IdfObject> _restockingSchedule = translateAndMapModelObject(restockingSchedule.get());

    if( _restockingSchedule && _restockingSchedule->name() )
    {
      refrigerationWalkIn.setString(Refrigeration_WalkInFields::RestockingScheduleName,_restockingSchedule->name().get());
    }
  }

//AverageRefrigerantChargeInventory
  d = modelObject.averageRefrigerantChargeInventory();
  if (d) {
    refrigerationWalkIn.setDouble(Refrigeration_WalkInFields::AverageRefrigerantChargeInventory,d.get());
  }

//InsulatedFloorSurfaceArea
  d = modelObject.insulatedFloorSurfaceArea();
  if (d) {
    refrigerationWalkIn.setDouble(Refrigeration_WalkInFields::InsulatedFloorSurfaceArea,d.get());
  }

//InsulatedFloorUValue
  d = modelObject.insulatedFloorUValue();
  if (d) {
    refrigerationWalkIn.setDouble(Refrigeration_WalkInFields::InsulatedFloorUValue,d.get());
  }

//ZoneBoundaries
  std::vector<RefrigerationWalkInZoneBoundary> zoneBoundaries = modelObject.zoneBoundaries();

  if( !zoneBoundaries.empty() )
  {
    for( const auto & _zoneBoundary : zoneBoundaries )
    {
      IdfExtensibleGroup eg = refrigerationWalkIn.pushExtensibleGroup();

      //ZoneName
        boost::optional<ThermalZone> thermalZone = _zoneBoundary.thermalZone();

        if( thermalZone )
        {
          boost::optional<IdfObject> _thermalZone = translateAndMapModelObject(thermalZone.get());

          if( _thermalZone && _thermalZone->name() )
          {
            eg.setString(Refrigeration_WalkInExtensibleFields::ZoneName,_thermalZone->name().get());
          }
        }

      //TotalInsulatedSurfaceAreaFacingZone
        d = _zoneBoundary.totalInsulatedSurfaceAreaFacingZone();
        if (d) {
          eg.setDouble(Refrigeration_WalkInExtensibleFields::TotalInsulatedSurfaceAreaFacingZone,d.get());
        }

      //InsulatedSurfaceUValueFacingZone
        d = _zoneBoundary.insulatedSurfaceUValueFacingZone();
        if (d) {
          eg.setDouble(Refrigeration_WalkInExtensibleFields::InsulatedSurfaceUValueFacingZone,d.get());
        }

      //AreaofGlassReachInDoorsFacingZone
        d = _zoneBoundary.areaofGlassReachInDoorsFacingZone();
        if (d) {
          eg.setDouble(Refrigeration_WalkInExtensibleFields::AreaofGlassReachInDoorsFacingZone,d.get());
        }

      //HeightofGlassReachInDoorsFacingZone
        d = _zoneBoundary.heightofGlassReachInDoorsFacingZone();
        if (d) {
          eg.setDouble(Refrigeration_WalkInExtensibleFields::HeightofGlassReachInDoorsFacingZone,d.get());
        }

      //GlassReachInDoorUValueFacingZone
        d = _zoneBoundary.glassReachInDoorUValueFacingZone();
        if (d) {
          eg.setDouble(Refrigeration_WalkInExtensibleFields::GlassReachInDoorUValueFacingZone,d.get());
        }

      //GlassReachInDoorOpeningScheduleNameFacingZone
        boost::optional<Schedule> glassReachInDoorOpeningScheduleFacingZone = _zoneBoundary.glassReachInDoorOpeningScheduleFacingZone();

        if( glassReachInDoorOpeningScheduleFacingZone )
        {
          boost::optional<IdfObject> _glassReachInDoorOpeningScheduleFacingZone = translateAndMapModelObject(glassReachInDoorOpeningScheduleFacingZone.get());

          if( _glassReachInDoorOpeningScheduleFacingZone && _glassReachInDoorOpeningScheduleFacingZone->name() )
          {
            eg.setString(Refrigeration_WalkInExtensibleFields::GlassReachInDoorOpeningScheduleNameFacingZone,_glassReachInDoorOpeningScheduleFacingZone->name().get());
          }
        }

      //AreaofStockingDoorsFacingZone
        d = _zoneBoundary.areaofStockingDoorsFacingZone();
        if (d) {
          eg.setDouble(Refrigeration_WalkInExtensibleFields::AreaofStockingDoorsFacingZone,d.get());
        }

      //HeightofStockingDoorsFacingZone
        d = _zoneBoundary.heightofStockingDoorsFacingZone();
        if (d) {
          eg.setDouble(Refrigeration_WalkInExtensibleFields::HeightofStockingDoorsFacingZone,d.get());
        }

      //StockingDoorUValueFacingZone
        d = _zoneBoundary.stockingDoorUValueFacingZone();
        if (d) {
          eg.setDouble(Refrigeration_WalkInExtensibleFields::StockingDoorUValueFacingZone,d.get());
        }

      //StockingDoorOpeningScheduleNameFacingZone
        boost::optional<Schedule> stockingDoorOpeningScheduleFacingZone = _zoneBoundary.stockingDoorOpeningScheduleFacingZone();

        if( stockingDoorOpeningScheduleFacingZone )
        {
          boost::optional<IdfObject> _stockingDoorOpeningScheduleFacingZone = translateAndMapModelObject(stockingDoorOpeningScheduleFacingZone.get());

          if( _stockingDoorOpeningScheduleFacingZone && _stockingDoorOpeningScheduleFacingZone->name() )
          {
            eg.setString(Refrigeration_WalkInExtensibleFields::StockingDoorOpeningScheduleNameFacingZone,_stockingDoorOpeningScheduleFacingZone->name().get());
          }
        }

      //StockingDoorOpeningProtectionTypeFacingZone
        s = _zoneBoundary.stockingDoorOpeningProtectionTypeFacingZone();
        if (s) {
          eg.setString(Refrigeration_WalkInExtensibleFields::StockingDoorOpeningProtectionTypeFacingZone,s.get());
        }
    }
  }

  return refrigerationWalkIn;

}
}
}
