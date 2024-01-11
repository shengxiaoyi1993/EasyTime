//#include "gtest/gtest.h"
//#include <iostream>
//#include <math.h>
//#include "toml/toml.h"


////#include "


//TEST(appendJPEG,multi_color_image){
//  std::string path_toml="test.toml";
//  Toml *d = new Toml(path_toml);

//  int ret_save=d->Save(path_toml);

//  EXPECT_EQ(0,ret_save);

//  std::string name_key="name";
//  std::string name_value="sxy";
//  std::string name_value_get="";

//  d->Set(name_key, name_value);
//  d->Get(name_key, name_value_get);
//  ASSERT_STREQ(name_value.c_str(),name_value_get.c_str());

//  std::string false_key="valid_true";
//  bool false_value=false;
//  bool false_value_get=false;
//  d->Set(false_key, false_value);
//  d->Get(false_key, false_value_get);
//  ASSERT_EQ(false_value,false_value_get);


//  std::string true_key="valid_true";
//  bool true_value=true;
//  bool true_value_get=true;
//  d->Set(true_key, true_value);
//  d->Get(true_key, true_value_get);
//  ASSERT_EQ(true_value,true_value_get);

//  std::string double_key="val_double";
//  double double_value=1.234;
//  double double_value_get=0;
//  d->Set(double_key, double_value);
//  d->Get(double_key, double_value_get);
//  double div=double_value-double_value_get;
//  div=div>0?div:-div;
//  // ASSERT_EQ(double_value,double_value_get);
//  ASSERT_LT(div,1e-5);

//  std::string int_key="val_int";
//  int int_value=1;
//  int int_value_get=0;
//  d->Set(int_key, int_value);
//  d->Get(int_key, int_value_get);
//  ASSERT_EQ(int_value,int_value_get);

//}




/// 继承于public TestWithParam<> 的设施 必须使用TEST_P
/// 继承于public TestWithParam<> 的设施 若没有使用INSTANTIATE_TEST_CASE_P定义参数，则不会执行
/// 继承于public TestWithParam<> 的设施 即使没有参数也无法使用TEST_F
///

#include "gtest/gtest.h"
#include <iostream>
using ::testing::TestWithParam;
using ::testing::Bool;
using ::testing::Values;
using ::testing::Combine;
using namespace std;

class funcTest : public TestWithParam< ::std::tr1::tuple<int , int> >
{
protected:
        virtual void SetUp()
        {
                a = ::std::tr1::get<0>(GetParam());
                b = ::std::tr1::get<1>(GetParam());
        }
        virtual void TearDown()
        {

        }
int a;
int b;
};

TEST_P(funcTest ,mytest__haha )
{
        cout << "a = "<<a<<", b = "<<b <<" is " <<max(a , b)<<endl;
}

//INSTANTIATE_TEST_CASE_P(MeaningfulTestParameters,
//    funcTest,
//    Combine(Values(1,3) , Values(1,2)));
