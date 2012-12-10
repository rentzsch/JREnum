#import <Foundation/Foundation.h>
#import "JREnum.h"

JREnumDeclare(SplitEnumWith1ConstantSansExplicitValues,
              SplitEnumWith1ConstantSansExplicitValues_Constant1);

JREnumDeclare(SplitEnumWith1ConstantWithExplicitValues,
              SplitEnumWith1ConstantWithExplicitValues_Constant1 = 42);

JREnumDeclare(TestClassState,
              TestClassState_Closed,
              TestClassState_Opening,
              TestClassState_Open,
              TestClassState_Closing);
