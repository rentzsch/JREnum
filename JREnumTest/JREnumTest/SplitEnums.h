#import <Foundation/Foundation.h>
#import "JREnum.h"

JREnumDeclare(SplitEnumWith1ConstantSansExplicitValues,
              SplitEnumWith1ConstantSansExplicitValues_Constant1);

JREnumDeclare(SplitEnumWith2ConstantsSansExplicitValues,
              SplitEnumWith2ConstantsSansExplicitValues_Constant1,
              SplitEnumWith2ConstantsSansExplicitValues_Constant2);

JREnumDeclare(SplitEnumWith1ConstantWithExplicitValues,
              SplitEnumWith1ConstantWithExplicitValues_Constant1 = 42);

JREnumDeclare(TestClassState,
              TestClassState_Closed,
              TestClassState_Opening,
              TestClassState_Open,
              TestClassState_Closing);

JREnumDeclare(Align,
              AlignLeft         = 0x00000001,
              AlignRight        = 0x00000010,
              AlignTop          = 0x00000100,
              AlignBottom       = 0x00001000,
              AlignTopLeft      = 0x00000101,
              AlignBottomLeft   = 0x00001001,
              AlignTopRight     = 0x00000110,
              AlignBottomRight  = 0x00001010,
              );