// JREnum.h semver:1.1.0
//   Original implementation by Benedict Cohen: http://benedictcohen.co.uk
//   Copyright (c) 2012-2013 Jonathan 'Wolf' Rentzsch: http://rentzsch.com
//   Some rights reserved: http://opensource.org/licenses/mit
//   https://github.com/rentzsch/JREnum

/**	Usage:
	Externally visible declarations: (Bitwise example shown)
	.h		JROptionsDeclare(YourType, YourTypeRed = 00000001, YourTypeGreen = 0x00000010, YourTypeBlue = 00000100);
	.m		JROptionsDefine(YourType);
	Private usage:
	.m		JROptions(YourType, YourTypeRed = 00000001, YourTypeGreen = 0x00000010, YourTypeBlue = 00000100);

	For the following TYPE_NAME_ = YourType    ENUM_VALUE could be... YourTypeGreen

	NSString* TYPE_NAME_ToString(ENUM_VALUE)
	YourTypeToString(YourTypeGreen) -> @"YourTypeGreen".

	YOUR_TYPE_ aPointer;
	BOOL TYPE_NAME_FromString(@"ENUM_VALUE", &aPointer)
	Attempts to return the enum's int value given its label. For example 
	
	YourType z = 0;
	BOOL x = YourTypeFromString(@"YourTypeGreen", &z) 
	
	"x" would evaluate to YES and "z" would be to 16 (aka 0x00000010).
	This function returns NO if the label for the enum type is unknown. 
	YourTypeFromString(@"lackadaisical", &z) would return NO and leave "z" untouched.

	NSDictionary* TYPE_NAME_ByValue() -> 	(NSDictionary*) @{ NSNumber VALUE : NSString* LABEL }
		Used by TYPE_NAME_ToString(). If multiple overlapping values exist, last-write-wins.

	NSDictionary* TYPE_NAME_ByLabel() -> 	(NSDictionary*) @{ NSString* LABEL : NSNumber VALUE }
		Used by TYPE_NAME_FromString().  Use to enumerate an enum's labels and values at runtime.
*/

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
    _Pragma("clang diagnostic push") \
    _Pragma("clang diagnostic ignored \"-Wunused-value\"") \
    static NSString *_##ENUM_TYPENAME##_constants_string = @"" #ENUM_CONSTANTS; \
    _Pragma("clang diagnostic pop")

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

//--  NS_OPTIONS macro for BITWISE ENUMS

#define JROptions(ENUM_TYPENAME, ENUM_CONSTANTS...)                             \
    typedef NS_OPTIONS(NSUInteger, ENUM_TYPENAME) { ENUM_CONSTANTS  };          \
    static NSString *_##ENUM_TYPENAME##_constants_string = @"" #ENUM_CONSTANTS; \
   _JROptions_GenerateImplementation(ENUM_TYPENAME)

//--

#define JROptionsDeclare(ENUM_TYPENAME, ENUM_CONSTANTS...)                                   \
    typedef NS_OPTIONS(NSUInteger, ENUM_TYPENAME) { ENUM_CONSTANTS    };                     \
    extern NSDictionary* ENUM_TYPENAME##ByValue();                                           \
    extern NSDictionary* ENUM_TYPENAME##ByLabel();                                           \
    extern NSString* ENUM_TYPENAME##ToString(int enumValue);                                 \
    extern BOOL ENUM_TYPENAME##FromString(NSString *enumLabel, ENUM_TYPENAME *enumValue);    \
    static NSString *_##ENUM_TYPENAME##_constants_string = @"" #ENUM_CONSTANTS;

#define JROptionsDefine(ENUM_TYPENAME) _JROptions_GenerateImplementation(ENUM_TYPENAME)

//--

#define _JROptions_GenerateImplementation(ENUM_TYPENAME)                                                      \
    NSArray* _JROptionsParse##ENUM_TYPENAME##ConstantsString() {                                              \
        NSArray *stringPairs = [_##ENUM_TYPENAME##_constants_string componentsSeparatedByString:@","];        \
        NSMutableArray *labelsAndValues = NSMutableArray.new;                                                 \
        NSNumber *lastValue = @(0);                                                                           \
        for (NSString *stringPair in stringPairs) {                                                           \
            NSArray *labelAndValueString = [stringPair componentsSeparatedByString:@"="];                     \
            NSString *label = labelAndValueString[0];                                                         \
            label = [label stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];          \
            NSString *valueString = labelAndValueString.count > 1 ? labelAndValueString[1] : nil;             \
            NSScanner *scanner = [NSScanner scannerWithString:valueString];                                   \
            NSUInteger aNumericValue;                                                                         \
            [scanner scanHexInt:(NSInteger)&aNumericValue];                                                   \
            NSNumber *value = valueString ? [NSNumber numberWithInt:aNumericValue]                            \
                                          : @([lastValue intValue]+1);                                        \
            lastValue = value;                                                                                \
            [labelsAndValues addObject:label];                                                                \
            [labelsAndValues addObject:value];                                                                \
        }                                                                                                     \
        if (0) NSLog (@"DEBUG: stringPairs:%@ labelsandVals:%@", stringPairs, labelsAndValues);               \
        return labelsAndValues;                                                                               \
    }                                                                                                         \
    NSDictionary* ENUM_TYPENAME##ByValue() {                                                                  \
        NSArray *constants = _JROptionsParse##ENUM_TYPENAME##ConstantsString();                               \
        NSMutableDictionary *result = NSMutableDictionary.new;                                                \
        for (NSUInteger i = 0; i < constants.count; i += 2)                                                   \
            [result setObject:constants[i] forKey:constants[i+1]];                                            \
        return result;                                                                                        \
    }                                                                                                         \
    NSDictionary* ENUM_TYPENAME##ByLabel() {                                                                  \
        NSArray *constants = _JROptionsParse##ENUM_TYPENAME##ConstantsString();                               \
        NSMutableDictionary *result = NSMutableDictionary.new;                                                \
        for (NSUInteger i = 0; i < constants.count; i += 2)                                                   \
            [result setObject:(NSNumber*)constants[i+1] forKey:(NSString*)constants[i]];                      \
        return result;                                                                                        \
    }                                                                                                         \
    NSString* ENUM_TYPENAME##ToString(int enumValue) {                                                        \
        NSString *result = [ENUM_TYPENAME##ByValue() objectForKey:[NSNumber numberWithInt:enumValue]];        \
        return result ?: [NSString stringWithFormat:@"<unknown "#ENUM_TYPENAME": %d>", enumValue];            \
    }                                                                                                         \
    BOOL ENUM_TYPENAME##FromString(NSString *enumLabel, ENUM_TYPENAME *enumValue) {                           \
        NSNumber *value = [ENUM_TYPENAME##ByLabel() objectForKey:enumLabel];                                  \
        if (value) { *enumValue = (ENUM_TYPENAME)[value intValue];    return YES;    } else return NO;        \
    }
