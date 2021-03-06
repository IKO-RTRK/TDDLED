#include "../unity/unity_fixture.h"
#include "../src/LedDriver.h"

static uint16_t virtualLeds;

TEST_GROUP(LedDriver);

TEST_GROUP_RUNNER(LedDriver)
{
	RUN_TEST_CASE(LedDriver, LedsOffAfterCreate);
	RUN_TEST_CASE(LedDriver, TurnOnLedOne);
	RUN_TEST_CASE(LedDriver, TurnOffLedOne);
	RUN_TEST_CASE(LedDriver, TurnOnMultipleLeds);
	RUN_TEST_CASE(LedDriver, TurnOffAnyLed);
	RUN_TEST_CASE(LedDriver, AllOn);
	
	RUN_TEST_CASE(LedDriver, LedMemoryIsNotReadable);
	RUN_TEST_CASE(LedDriver, UpperAndLowerBounds);

	RUN_TEST_CASE(LedDriver, OutOfBoundsTurnOnDoesNoHarm);
	RUN_TEST_CASE(LedDriver, OutOfBoundsTurnOffDoesNoHarm);

	RUN_TEST_CASE(LedDriver, IsOn);
	RUN_TEST_CASE(LedDriver, OutOfBoundsLedsAreAlwaysOff);

	RUN_TEST_CASE(LedDriver, IsOff);
	
	RUN_TEST_CASE(LedDriver, TurnOffMultipleLeds);

	RUN_TEST_CASE(LedDriver, AllOff);
}


TEST_SETUP(LedDriver)
{
	LedDriver_Create(&virtualLeds);
}

TEST_TEAR_DOWN(LedDriver)
{
}


TEST(LedDriver, LedsOffAfterCreate)
{
	uint16_t virtualLeds = 0xffff;
	LedDriver_Create(&virtualLeds);
	TEST_ASSERT_EQUAL_HEX16(0, virtualLeds);
}

TEST(LedDriver, TurnOnLedOne)
{
	LedDriver_TurnOn(1);
	TEST_ASSERT_EQUAL_HEX16(1, virtualLeds);
}

TEST(LedDriver, TurnOffLedOne)
{
	LedDriver_TurnOn(1);
	LedDriver_TurnOff(1);
	TEST_ASSERT_EQUAL_HEX16(0, virtualLeds);
}

TEST(LedDriver, TurnOnMultipleLeds)
{
	LedDriver_TurnOn(9);
	LedDriver_TurnOn(8);
	TEST_ASSERT_EQUAL_HEX16(0x180, virtualLeds);
}


// ovo je test 5 a ne 6 kako sam mislio ranije :D
TEST(LedDriver, TurnOffAnyLed)
{
	LedDriver_TurnAllOn();
	LedDriver_TurnOff(8);
	TEST_ASSERT_EQUAL_HEX16(0xff7f, virtualLeds);
}

TEST(LedDriver, AllOn)
{
	LedDriver_TurnAllOn();
	TEST_ASSERT_EQUAL_HEX16(0xffff, virtualLeds);
}


// sad vidimo da test prolazi!
TEST(LedDriver, LedMemoryIsNotReadable)
{
	virtualLeds = 0xffff;
	LedDriver_TurnOn(8);
	TEST_ASSERT_EQUAL_HEX16(0x80, virtualLeds);
}

// uvijek treba provjeriti granice... sjetimo se faktoriela :)
TEST(LedDriver, UpperAndLowerBounds)
{
	LedDriver_TurnOn(1);
	LedDriver_TurnOn(16);
	TEST_ASSERT_EQUAL_HEX16(0x8001, virtualLeds);
}

// Idemo mali rename OutOfBoundsChangesNothing -> OutOfBoundsTurnOnDoesNoHarm
TEST(LedDriver, OutOfBoundsTurnOnDoesNoHarm)
{
	LedDriver_TurnOn(-1);
	LedDriver_TurnOn(0);
	LedDriver_TurnOn(17);
	LedDriver_TurnOn(3141); 
	TEST_ASSERT_EQUAL_HEX16(0, virtualLeds);
}



// Normalno da ima, nismo ukljucili nista od ledica :)
TEST(LedDriver, OutOfBoundsTurnOffDoesNoHarm)
{
	LedDriver_TurnAllOn();

	LedDriver_TurnOff(-1);
	LedDriver_TurnOff(0);
	LedDriver_TurnOff(17);
	LedDriver_TurnOff(3141);
	TEST_ASSERT_EQUAL_HEX16(0xffff, virtualLeds);
}


TEST(LedDriver, IsOn)
{
	TEST_ASSERT_FALSE(LedDriver_IsOn(11));
	LedDriver_TurnOn(11);
	TEST_ASSERT_TRUE(LedDriver_IsOn(11));
}



TEST(LedDriver, OutOfBoundsLedsAreAlwaysOff)
{
	TEST_ASSERT_FALSE(LedDriver_IsOn(0));
	TEST_ASSERT_FALSE(LedDriver_IsOn(17));

	// DODAO
	TEST_ASSERT_TRUE(LedDriver_IsOff(0));
	TEST_ASSERT_TRUE(LedDriver_IsOff(17));
}

TEST(LedDriver, IsOff)
{
	TEST_ASSERT_TRUE(LedDriver_IsOff(12));
	LedDriver_TurnOn(12);
	TEST_ASSERT_FALSE(LedDriver_IsOff(12));
}


// ovaj i ocekujemo da ce proci jer je turnoff vec napravljena da to radi...
TEST(LedDriver, TurnOffMultipleLeds)
{
	LedDriver_TurnAllOn();
	LedDriver_TurnOff(9);
	LedDriver_TurnOff(8);
	TEST_ASSERT_EQUAL_HEX16((~0x180)&0xffff, virtualLeds);
}


TEST(LedDriver, AllOff)
{
	LedDriver_TurnAllOn();
	LedDriver_TurnAllOff();
	TEST_ASSERT_EQUAL_HEX16(0, virtualLeds);
}
