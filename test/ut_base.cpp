#include <stdlib.h>
#include <iostream>
#include <thread>
#include <chrono>

#include <state.h>

#include "elem.h"
#include "env.h"
#include "mchromo.h"
#include "chromo.h"

#include <cppunit/extensions/HelperMacros.h>

using namespace desmag;


class Ut_Base : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(Ut_Base);
//    CPPUNIT_TEST(test_Cre);
    CPPUNIT_TEST(test_Mut_Add);
    CPPUNIT_TEST_SUITE_END();
public:
    virtual void setUp();
    virtual void tearDown();
private:
    void test_Cre();
    void test_Mut_Add();
};

CPPUNIT_TEST_SUITE_REGISTRATION( Ut_Base );

void Ut_Base::setUp()
{
}

void Ut_Base::tearDown()
{
    CPPUNIT_ASSERT_EQUAL_MESSAGE("tearDown", 0, 0);
}


void Ut_Base::test_Cre()
{
    printf("\n === Test of creation of hier\n");
    Env env();
    System root("root", nullptr);
    TMut mut(ENt_Node);

    desa::TState<TMut>* ctr = new desa::TState<TMut>("St_ctr", nullptr, mut);
    root.AddComp(ctr);

    Elem* elem = new Elem("Root", nullptr);
    root.AddComp(*elem);
    bool res = Connect(ctr->Output(), *(elem->mInpMut));
    CPPUNIT_ASSERT_MESSAGE("Fail to connect St_ctr", res);

    root.Run();


    //CPPUNIT_ASSERT_MESSAGE("Fail to get tst1", tst2 != 0);
}

void Ut_Base::test_Mut_Add()
{
    printf("\n === Test of mutating: adding comp\n");

    Env env("./ut_cre_1.xml");
    env.ConstructSystem();
    env.Root()->Run();

    //CPPUNIT_ASSERT_MESSAGE("Fail to get tst1", tst2 != 0);
}

