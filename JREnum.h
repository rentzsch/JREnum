// JREnum.h semver:1.1
//   Original implementation by Benedict Cohen: http://benedictcohen.co.uk
//   Copyright (c) 2012-2014 Jonathan 'Wolf' Rentzsch: http://rentzsch.com
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
    _Pragma("clang diagnostic ignored \"-Wunused-variable\"") \
    static NSString *_##ENUM_TYPENAME##_constants_string = @"" #ENUM_CONSTANTS; \
    _Pragma("clang diagnostic pop")

#define JREnumDefine(ENUM_TYPENAME) \
    _JREnum_GenerateImplementation(ENUM_TYPENAME)

#ifndef JRAUTORELEASE
    #ifndef __has_feature
        #define __has_feature(x) 0
    #endif
    #if __has_feature(objc_arc)
        #define JRAUTORELEASE(OBJ) (OBJ)
    #else
        #define JRAUTORELEASE(OBJ) [(OBJ) autorelease]
    #endif
#endif

//--

#define _JREnum_GenerateImplementation(ENUM_TYPENAME)  \
    NSArray* _JREnumParse##ENUM_TYPENAME##ConstantsString() {	\
        NSString *constantsString = _##ENUM_TYPENAME##_constants_string; \
        constantsString = [[constantsString componentsSeparatedByCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]] componentsJoinedByString:@""]; \
        if ([constantsString hasSuffix:@","]) { \
            constantsString = [constantsString substringToIndex:[constantsString length]-1]; \
        } \
        NSArray *stringPairs = [constantsString componentsSeparatedByString:@","];	\
        NSMutableArray *labelsAndValues = [NSMutableArray arrayWithCapacity:[stringPairs count]];	\
        int nextDefaultValue = 0;	\
        for (NSString *stringPair in stringPairs) {	\
            NSArray *labelAndValueString = [stringPair componentsSeparatedByString:@"="];	\
            NSString *label = [labelAndValueString objectAtIndex:0];	\
            NSString *valueString = [labelAndValueString count] > 1 ? [labelAndValueString objectAtIndex:1] : nil;	\
            int value; \
            if (valueString) { \
                if ([valueString hasPrefix:@"0x"]) { \
                    [[NSScanner scannerWithString:valueString] scanHexInt:(unsigned int*)&value]; \
                } else { \
                    value = [valueString intValue]; \
                } \
            } else { \
                value = nextDefaultValue; \
            } \
            nextDefaultValue = value + 1;	\
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