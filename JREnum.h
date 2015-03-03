// JREnum.h semver:1.1
//   Original implementation by Benedict Cohen: http://benedictcohen.co.uk
//   Copyright (c) 2012-2014 Jonathan 'Wolf' Rentzsch: http://rentzsch.com
//   Some rights reserved: http://opensource.org/licenses/mit
//   https://github.com/rentzsch/JREnum

static NSArray* _JRPrivate_ParseEnumFromString(NSString *rawString) {
    NSString *enumString = [[rawString componentsSeparatedByCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]] componentsJoinedByString:@""];
    if ([enumString hasSuffix:@","]) {
        enumString = [enumString substringToIndex:[enumString length]-1];
    }
    NSArray *stringPairs = [enumString componentsSeparatedByString:@","];
    NSMutableArray *labelsAndValues = [NSMutableArray arrayWithCapacity:[stringPairs count]];
    int nextDefaultValue = 0;
    for (NSString *stringPair in stringPairs) {
        NSArray *labelAndValueString = [stringPair componentsSeparatedByString:@"="];
        NSString *label = [labelAndValueString objectAtIndex:0];
        NSString *valueString = [labelAndValueString count] > 1 ? [labelAndValueString objectAtIndex:1] : nil;
        int value;
        if (valueString) {
            NSRange shiftTokenRange = [valueString rangeOfString:@"<<"];
            if (shiftTokenRange.location != NSNotFound) {
                valueString = [valueString substringFromIndex:shiftTokenRange.location + 2];
                value = 1 << [valueString intValue];
            } else if ([valueString hasPrefix:@"0x"]) {
                [[NSScanner scannerWithString:valueString] scanHexInt:(unsigned int*)&value];
            } else {
                value = [valueString intValue];
            }
        } else {
            value = nextDefaultValue;
        }
        nextDefaultValue = value + 1;
        [labelsAndValues addObject:label];
        [labelsAndValues addObject:[NSNumber numberWithInt:value]];
    }
    return labelsAndValues;
}

static NSDictionary* _JRPrivate_EnumByValue(NSArray *constants) {
    NSMutableDictionary *result = [NSMutableDictionary dictionaryWithCapacity:[constants count] / 2];
    for (NSUInteger i = 0; i < [constants count]; i += 2) {
        NSString *label = [constants objectAtIndex:i];
        NSNumber *value = [constants objectAtIndex:i+1];
        [result setObject:label forKey:value];
    }
    return result;
}

static NSDictionary* _JRPrivate_EnumByLabel(NSArray *constants) {
    NSMutableDictionary *result = [NSMutableDictionary dictionaryWithCapacity:[constants count] / 2];
    for (NSUInteger i = 0; i < [constants count]; i += 2) {
        NSString *label = [constants objectAtIndex:i];
        NSNumber *value = [constants objectAtIndex:i+1];
        [result setObject:value forKey:label];
    }
    return result;
}

static NSString* _JRPrivate_EnumToString(NSArray *constants, NSString *enumTypeName, int enumValue) {
    NSString *result = [_JRPrivate_EnumByValue(constants) objectForKey:[NSNumber numberWithInt:enumValue]];
    if (!result) {
        result = [NSString stringWithFormat:@"<unknown %@: %d>", enumTypeName, enumValue];
    }
    return result;
}

static BOOL _JRPrivate_EnumFromString(NSArray *constants, NSString *enumLabel, NSInteger *enumValue) {
    NSNumber *value = [_JRPrivate_EnumByLabel(constants) objectForKey:enumLabel];
    if (value) {
        *enumValue = [value integerValue];
        return YES;
    } else {
        return NO;
    }
}



#define JREnum(ENUM_TYPENAME, ENUM_CONSTANTS...)    \
typedef NS_ENUM(NSInteger, ENUM_TYPENAME) {  \
    ENUM_CONSTANTS  \
};    \
\
\
static NSArray* _##ENUM_TYPENAME##KeysAndValues() {	\
    NSString *constantsString = @"" #ENUM_CONSTANTS; \
    return _JRPrivate_ParseEnumFromString(constantsString); \
} \
\
static NSDictionary* ENUM_TYPENAME##ByValue() {	\
    NSArray *constants = _##ENUM_TYPENAME##KeysAndValues();	\
    return _JRPrivate_EnumByValue(constants); \
}	\
\
static NSDictionary* ENUM_TYPENAME##ByLabel() {	\
    NSArray *constants = _##ENUM_TYPENAME##KeysAndValues();	\
    return _JRPrivate_EnumByLabel(constants); \
}	\
\
static NSString* ENUM_TYPENAME##ToString(int enumValue) {	\
    NSArray *constants = _##ENUM_TYPENAME##KeysAndValues();	\
    NSString *enumTypeName = @"" #ENUM_TYPENAME; \
    return _JRPrivate_EnumToString(constants, enumTypeName, enumValue); \
}	\
\
static BOOL ENUM_TYPENAME##FromString(NSString *enumLabel, ENUM_TYPENAME *enumValue) {	\
    NSArray *constants = _##ENUM_TYPENAME##KeysAndValues();	\
    return _JRPrivate_EnumFromString(constants, enumLabel, enumValue); \
}

//--

#define JREnumDeclare(ENUM_TYPENAME, ENUM_CONSTANTS...) \
    _Pragma("GCC warning \"JREnumDeclare is deprecated and should be replaced with JREnum.\"") \
    JREnum(ENUM_TYPENAME, ENUM_CONSTANTS)

//--


#define JREnumDefine(ENUM_TYPENAME) \
    _Pragma("GCC warning \"JREnumDefine is deprecated and should be removed.\"") \

//--
