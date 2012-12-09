// JREnum.h semver:0.1
//   Copyright (c) 2012 Benedict Cohen: http://benedictcohen.co.uk
//   Some rights reserved: http://opensource.org/licenses/mit
//   https://github.com/rentzsch/JREnum

#define JREnum(ENUM_NAME, ENUM_VALUES...)                                                                   \
    typedef enum {                                                                                          \
        ENUM_VALUES                                                                                         \
    } ENUM_NAME;                                                                                            \
                                                                                                            \
    static NSString* NSStringFrom##ENUM_NAME(int enumValue) {                                               \
        static dispatch_once_t sGuard;                                                                      \
        static NSMutableDictionary *sEnumsByValue = nil;                                                    \
        dispatch_once(&sGuard, ^{                                                                           \
            sEnumsByValue = [NSMutableDictionary dictionary];                                               \
            NSArray *enumPairs = [@"" #ENUM_VALUES componentsSeparatedByString:@","];                       \
            int lastValue = -1;                                                                             \
            for (NSString *enumPair in enumPairs) {                                                         \
                NSArray *labelAndValue = [enumPair componentsSeparatedByString:@"="];                       \
                NSString *label = [labelAndValue objectAtIndex:0];                                          \
                BOOL hasExplictValue = [labelAndValue count] > 1;                                           \
                int value = (hasExplictValue) ? [[labelAndValue objectAtIndex:1] intValue] : lastValue + 1; \
                [sEnumsByValue setObject:label forKey:[NSNumber numberWithInt:value]];                      \
                lastValue = value;                                                                          \
            }                                                                                               \
        });                                                                                                 \
        NSString *result = [sEnumsByValue objectForKey:[NSNumber numberWithInt:enumValue]];                 \
        if (!result) {                                                                                      \
            result = [NSString stringWithFormat:@"<unknown "#ENUM_NAME": %d>", enumValue];                  \
        }                                                                                                   \
        return result;                                                                                      \
    }

#define JREnumDeclare(ENUM_NAME, ENUM_VALUES...)                                                            \
    typedef enum {                                                                                          \
        ENUM_VALUES                                                                                         \
    } ENUM_NAME;                                                                                            \
    extern NSString* NSStringFrom##ENUM_NAME(int enumValue);                                                \
    static NSString *impl_str_##ENUM_NAME = @"" #ENUM_VALUES

#define JREnumDefine(ENUM_NAME)                                                                             \
    NSString* NSStringFrom##ENUM_NAME(int enumValue) {                                                      \
        static dispatch_once_t sGuard;                                                                      \
        static NSMutableDictionary *sEnumsByValue = nil;                                                    \
        dispatch_once(&sGuard, ^{                                                                           \
            sEnumsByValue = [NSMutableDictionary dictionary];                                               \
            NSArray *enumPairs = [impl_str_##ENUM_NAME componentsSeparatedByString:@","];                   \
            int lastValue = -1;                                                                             \
            for (NSString *enumPair in enumPairs) {                                                         \
                NSArray *labelAndValue = [enumPair componentsSeparatedByString:@"="];                       \
                NSString *label = [labelAndValue objectAtIndex:0];                                          \
                BOOL hasExplictValue = [labelAndValue count] > 1;                                           \
                int value = (hasExplictValue) ? [[labelAndValue objectAtIndex:1] intValue] : lastValue + 1; \
                [sEnumsByValue setObject:label forKey:[NSNumber numberWithInt:value]];                      \
                lastValue = value;                                                                          \
            }                                                                                               \
        });                                                                                                 \
        NSString *result = [sEnumsByValue objectForKey:[NSNumber numberWithInt:enumValue]];                 \
        if (!result) {                                                                                      \
            result = [NSString stringWithFormat:@"<unknown MyClassState: %d>", enumValue];                  \
        }                                                                                                   \
        return result;                                                                                      \
    }
