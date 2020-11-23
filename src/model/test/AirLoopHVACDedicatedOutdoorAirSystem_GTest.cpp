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

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../AirLoopHVACDedicatedOutdoorAirSystem.hpp"
#include "../AirLoopHVACDedicatedOutdoorAirSystem_Impl.hpp"
#include "../AirLoopHVACOutdoorAirSystem.hpp"
#include "../AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "../ControllerOutdoorAir.hpp"
#include "../ControllerOutdoorAir_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../AirLoopHVAC_Impl.hpp"
#include "../ScheduleConstant.hpp"
#include "../ScheduleConstant_Impl.hpp"

#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

TEST_F(ModelFixture,AirLoopHVACDedicatedOutdoorAirSystem_AirLoopHVACDedicatedOutdoorAirSystem)
{
  Model model;
  ControllerOutdoorAir controller(model);
  AirLoopHVACOutdoorAirSystem oaSystem(model, controller);

  AirLoopHVACDedicatedOutdoorAirSystem doaSystem(model, oaSystem);

  ASSERT_TRUE(doaSystem.outdoorAirSystem().optionalCast<AirLoopHVACOutdoorAirSystem>());
  ASSERT_FALSE(doaSystem.availabilitySchedule());
  EXPECT_EQ(4.5, doaSystem.preheatDesignTemperature());
  EXPECT_EQ(0.004, doaSystem.preheatDesignHumidityRatio());
  EXPECT_EQ(17.5, doaSystem.precoolDesignTemperature());
  EXPECT_EQ(0.012, doaSystem.precoolDesignHumidityRatio());
  EXPECT_EQ(0, doaSystem.numberofAirLoops());
  EXPECT_EQ(0, doaSystem.airLoops().size());
}

TEST_F(ModelFixture, AirLoopHVACDedicatedOutdoorAirSystem_GettersSetters) {
  Model model;
  ControllerOutdoorAir controller(model);
  AirLoopHVACOutdoorAirSystem oaSystem(model, controller);

  AirLoopHVACDedicatedOutdoorAirSystem doaSystem(model, oaSystem);

  ControllerOutdoorAir controller2(model);
  AirLoopHVACOutdoorAirSystem oaSystem2(model, controller2);
  oaSystem2.setName("New System");
  ScheduleConstant sched(model);
  sched.setValue(0.5);
  AirLoopHVAC airLoop(model);

  EXPECT_TRUE(doaSystem.setOutdoorAirSystem(oaSystem2));
  EXPECT_TRUE(doaSystem.setAvailabilitySchedule(sched));
  EXPECT_TRUE(doaSystem.setPreheatDesignTemperature(5.0));
  EXPECT_TRUE(doaSystem.setPreheatDesignHumidityRatio(0.0035));
  EXPECT_TRUE(doaSystem.setPrecoolDesignTemperature(18.2));
  EXPECT_TRUE(doaSystem.setPrecoolDesignHumidityRatio(0.0112));
  EXPECT_TRUE(doaSystem.addAirLoop(airLoop));

  ASSERT_TRUE(doaSystem.outdoorAirSystem().optionalCast<AirLoopHVACOutdoorAirSystem>());
  EXPECT_EQ("New System", doaSystem.outdoorAirSystem().nameString());
  boost::optional<Schedule> schedule = doaSystem.availabilitySchedule();
  ASSERT_TRUE(schedule);
  boost::optional<ScheduleConstant> scheduleConstant = schedule.get().optionalCast<ScheduleConstant>();
  ASSERT_TRUE(scheduleConstant);
  EXPECT_EQ((*scheduleConstant).value(), 0.5);
  EXPECT_EQ(5.0, doaSystem.preheatDesignTemperature());
  EXPECT_EQ(0.0035, doaSystem.preheatDesignHumidityRatio());
  EXPECT_EQ(18.2, doaSystem.precoolDesignTemperature());
  EXPECT_EQ(0.0112, doaSystem.precoolDesignHumidityRatio());
  EXPECT_EQ(1, doaSystem.numberofAirLoops());
  EXPECT_EQ(1, doaSystem.airLoops().size());
  EXPECT_TRUE(doaSystem.removeAirLoop(airLoop));
  EXPECT_EQ(0, doaSystem.numberofAirLoops());
  EXPECT_EQ(0, doaSystem.airLoops().size());

  std::vector<AirLoopHVAC> airLoopHVACs;
  airLoopHVACs.push_back(airLoop);
  EXPECT_TRUE(doaSystem.addAirLoops(airLoopHVACs));
  EXPECT_EQ(1, doaSystem.numberofAirLoops());
  EXPECT_EQ(1, doaSystem.airLoops().size());
  doaSystem.removeAllAirLoops();
  EXPECT_EQ(0, doaSystem.numberofAirLoops());
  EXPECT_EQ(0, doaSystem.airLoops().size());
  EXPECT_FALSE(doaSystem.removeAirLoop(0));

  AirLoopHVAC airLoop2(model);
  airLoopHVACs.push_back(airLoop2);

  EXPECT_TRUE(doaSystem.addAirLoops(airLoopHVACs));
  EXPECT_EQ(2, doaSystem.numberofAirLoops());
  EXPECT_EQ(2, doaSystem.airLoops().size());
  EXPECT_TRUE(doaSystem.removeAirLoop(1));
  EXPECT_EQ(1, doaSystem.numberofAirLoops());
  EXPECT_EQ(1, doaSystem.airLoops().size());
}

TEST_F(ModelFixture, AirLoopHVACDedicatedOutdoorAirSystem_clone) {
  Model model;
  ControllerOutdoorAir controller(model);
  AirLoopHVACOutdoorAirSystem oaSystem(model, controller);
  AirLoopHVAC airLoop(model);
  AirLoopHVAC airLoop2(model);
  std::vector<AirLoopHVAC> airLoopHVACs;
  airLoopHVACs.push_back(airLoop);
  airLoopHVACs.push_back(airLoop2);

  AirLoopHVACDedicatedOutdoorAirSystem doaSystem(model, oaSystem);
  doaSystem.addAirLoops(airLoopHVACs);

  EXPECT_EQ(2, doaSystem.numberofAirLoops());
  EXPECT_EQ(2, doaSystem.airLoops().size());
  EXPECT_EQ(2, model.getConcreteModelObjects<AirLoopHVAC>().size());
  EXPECT_EQ(1, model.getConcreteModelObjects<AirLoopHVACDedicatedOutdoorAirSystem>().size());

  auto doaSystemClone = doaSystem.clone(model).cast<AirLoopHVACDedicatedOutdoorAirSystem>();

  EXPECT_EQ(2, doaSystemClone.numberofAirLoops());
  EXPECT_EQ(2, doaSystemClone.airLoops().size());
  EXPECT_EQ(2, model.getConcreteModelObjects<AirLoopHVAC>().size());
  EXPECT_EQ(2, model.getConcreteModelObjects<AirLoopHVACDedicatedOutdoorAirSystem>().size());
}