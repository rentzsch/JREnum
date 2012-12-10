#import <Foundation/Foundation.h>
#import "SplitEnums.h"

JREnum(EnumWith1ConstantSansExplicitValues,
       EnumWith1ConstantSansExplicitValues_Constant1);

JREnum(EnumWith1ConstantWithExplicitValues,
       EnumWith1ConstantWithExplicitValues_Constant1 = 42);

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        {{
            assert([EnumWith1ConstantSansExplicitValuesByLabel() count] == 1);
            assert([[EnumWith1ConstantSansExplicitValuesByLabel() objectForKey:@"EnumWith1ConstantSansExplicitValues_Constant1"] isEqual:[NSNumber numberWithInt:0]]);
            
            assert([EnumWith1ConstantSansExplicitValuesByValue() count] == 1);
            assert([[EnumWith1ConstantSansExplicitValuesByValue() objectForKey:[NSNumber numberWithInt:0]] isEqual:@"EnumWith1ConstantSansExplicitValues_Constant1"]);
            
            EnumWith1ConstantSansExplicitValues a = 0;
            
            assert(EnumWith1ConstantSansExplicitValues_Constant1 == a);
            assert([@"EnumWith1ConstantSansExplicitValues_Constant1" isEqualToString:EnumWith1ConstantSansExplicitValuesToString(a)]);
            assert(EnumWith1ConstantSansExplicitValuesFromString(EnumWith1ConstantSansExplicitValuesToString(EnumWith1ConstantSansExplicitValues_Constant1), &a));
            assert(EnumWith1ConstantSansExplicitValues_Constant1 == a);
            
            a++;
            assert([@"<unknown EnumWith1ConstantSansExplicitValues: 1>" isEqualToString:EnumWith1ConstantSansExplicitValuesToString(a)]);
            assert(!EnumWith1ConstantSansExplicitValuesFromString(@"foo", &a));
        }}
        {{
            SplitEnumWith1ConstantSansExplicitValues a = 0;
            
            assert(SplitEnumWith1ConstantSansExplicitValues_Constant1 == a);
            assert([@"SplitEnumWith1ConstantSansExplicitValues_Constant1" isEqualToString:SplitEnumWith1ConstantSansExplicitValuesToString(a)]);
            assert(SplitEnumWith1ConstantSansExplicitValuesFromString(SplitEnumWith1ConstantSansExplicitValuesToString(SplitEnumWith1ConstantSansExplicitValues_Constant1), &a));
            assert(SplitEnumWith1ConstantSansExplicitValues_Constant1 == a);
            
            a++;
            assert([@"<unknown SplitEnumWith1ConstantSansExplicitValues: 1>" isEqualToString:SplitEnumWith1ConstantSansExplicitValuesToString(a)]);
            assert(!SplitEnumWith1ConstantSansExplicitValuesFromString(@"foo", &a));
        }}
        {{
            SplitEnumWith1ConstantWithExplicitValues a = 42;
            
            assert(SplitEnumWith1ConstantWithExplicitValues_Constant1 == a);
            assert([@"SplitEnumWith1ConstantWithExplicitValues_Constant1" isEqualToString:SplitEnumWith1ConstantWithExplicitValuesToString(a)]);
            assert(SplitEnumWith1ConstantWithExplicitValuesFromString(SplitEnumWith1ConstantWithExplicitValuesToString(SplitEnumWith1ConstantWithExplicitValues_Constant1), &a));
            assert(SplitEnumWith1ConstantWithExplicitValues_Constant1 == a);
            
            a++;
            assert([@"<unknown SplitEnumWith1ConstantWithExplicitValues: 43>" isEqualToString:SplitEnumWith1ConstantWithExplicitValuesToString(a)]);
            assert(!SplitEnumWith1ConstantWithExplicitValuesFromString(@"foo", &a));
        }}
    }
    printf("success\n");
    return 0;
}

