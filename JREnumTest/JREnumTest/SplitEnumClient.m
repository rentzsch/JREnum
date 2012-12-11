#import "SplitEnumClient.h"
#import "SplitEnums.h"

@implementation SplitEnumClient

- (void)test {
    SplitEnumWith1ConstantSansExplicitValues a = SplitEnumWith1ConstantSansExplicitValues_Constant1;
    NSLog(@"%@", SplitEnumWith1ConstantSansExplicitValuesToString(a));
}

@end
