// JREnum.h semver:0.2
//   Original implementation by Benedict Cohen: http://benedictcohen.co.uk
//   Copyright (c) 2012 Jonathan 'Wolf' Rentzsch: http://rentzsch.com
//   Some rights reserved: http://opensource.org/licenses/mit
//   https://github.com/rentzsch/JREnum

#define JREnum(ENUM_TYPENAME, ENUM_CONSTANTS...)    \
    typedef enum {  \
        ENUM_CONSTANTS  \
    } ENUM_TYPENAME;    \
    static NSString *_##ENUM_TYPENAME##_constants_string = @"" #ENUM_CONSTANTS; \
    _JREnum_GenerateImplementation(ENUM_TYPENAME)

//--

#define JREnumDeclare(ENUM_TYPENAME, ENUM_CONSTANTS...) \
    typedef enum {  \
        ENUM_CONSTANTS  \
    } ENUM_TYPENAME;    \
    extern NSDictionary* ENUM_TYPENAME##ByValue();  \
    extern NSDictionary* ENUM_TYPENAME##ByLabel();  \
    extern NSString* ENUM_TYPENAME##ToString(int enumValue);    \
    extern BOOL ENUM_TYPENAME##FromString(NSString *enumLabel, ENUM_TYPENAME *enumValue);   \
    static NSString *_##ENUM_TYPENAME##_constants_string = @"" #ENUM_CONSTANTS;

#define JREnumDefine(ENUM_TYPENAME) \
    _JREnum_GenerateImplementation(ENUM_TYPENAME)

//--

#define _JREnum_GenerateImplementation(ENUM_TYPENAME)  \
    NSArray* _JREnumParse##ENUM_TYPENAME##ConstantsString() {	\
        NSArray *stringPairs = [_##ENUM_TYPENAME##_constants_string componentsSeparatedByString:@","];	\
        NSMutableArray *labelsAndValues = [NSMutableArray arrayWithCapacity:[stringPairs count]];	\
        int lastValue = -1;	\
        for (NSString *stringPair in stringPairs) {	\
            NSArray *labelAndValueString = [stringPair componentsSeparatedByString:@"="];	\
            NSString *label = [labelAndValueString objectAtIndex:0];	\
            label = [label stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];    \
            NSString *valueString = [labelAndValueString count] > 1 ? [labelAndValueString objectAtIndex:1] : nil;	\
            int value = valueString ? [valueString intValue] : lastValue + 1;	\
            lastValue = value;	\
            [labelsAndValues addObject:label];	\
            [labelsAndValues addObject:[NSNumber numberWithInt:value]];	\
        }	\
        return labelsAndValues;	\
    }	\
        \
    NSDictionary* ENUM_TYPENAME##ByValue() {	\
        NSArray *constants = _JREnumParse##ENUM_TYPENAME##ConstantsString();	\
        NSMutableDictionary *result = [NSMutableDictionary dictionaryWithCapacity:[constants count] / 2];	\
        for (NSUInteger i = 0; i < [constants count]; i += 2) {	\
            NSString *label = [constants objectAtIndex:i];	\
            NSNumber *value = [constants objectAtIndex:i+1];	\
            [result setObject:label forKey:value];	\
        }	\
        return result;	\
    }	\
        \
    NSDictionary* ENUM_TYPENAME##ByLabel() {	\
        NSArray *constants = _JREnumParse##ENUM_TYPENAME##ConstantsString();	\
        NSMutableDictionary *result = [NSMutableDictionary dictionaryWithCapacity:[constants count] / 2];	\
        for (NSUInteger i = 0; i < [constants count]; i += 2) {	\
            NSString *label = [constants objectAtIndex:i];	\
            NSNumber *value = [constants objectAtIndex:i+1];	\
            [result setObject:value forKey:label];	\
        }	\
        return result;	\
    }	\
        \
    NSString* ENUM_TYPENAME##ToString(int enumValue) {	\
        NSString *result = [ENUM_TYPENAME##ByValue() objectForKey:[NSNumber numberWithInt:enumValue]];	\
        if (!result) {	\
            result = [NSString stringWithFormat:@"<unknown "#ENUM_TYPENAME": %d>", enumValue];	\
        }	\
        return result;	\
    }	\
        \
    BOOL ENUM_TYPENAME##FromString(NSString *enumLabel, ENUM_TYPENAME *enumValue) {	\
        NSNumber *value = [ENUM_TYPENAME##ByLabel() objectForKey:enumLabel];	\
        if (value) {	\
            *enumValue = (ENUM_TYPENAME)[value intValue];	\
            return YES;	\
        } else {	\
            return NO;	\
        }	\
    }
