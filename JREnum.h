// JREnum.h semver:1.1
//   Original implementation by Benedict Cohen: http://benedictcohen.co.uk
//   Copyright (c) 2012-2014 Jonathan 'Wolf' Rentzsch: http://rentzsch.com
//   Some rights reserved: http://opensource.org/licenses/mit
//   https://github.com/rentzsch/JREnum

static NSArray* _JRPrivate_ParseEnumLabelsAndValuesFromString(NSString *rawString) {
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

static NSDictionary* _JRPrivate_EnumByValue(NSArray *labelsAndValues) {
    NSMutableDictionary *result = [NSMutableDictionary dictionaryWithCapacity:[labelsAndValues count] / 2];
    for (NSUInteger i = 0; i < [labelsAndValues count]; i += 2) {
        NSString *label = [labelsAndValues objectAtIndex:i];
        NSNumber *value = [labelsAndValues objectAtIndex:i+1];
        [result setObject:label forKey:value];
    }
    return result;
}

static NSDictionary* _JRPrivate_EnumByLabel(NSArray *labelsAndValues) {
    NSMutableDictionary *result = [NSMutableDictionary dictionaryWithCapacity:[labelsAndValues count] / 2];
    for (NSUInteger i = 0; i < [labelsAndValues count]; i += 2) {
        NSString *label = [labelsAndValues objectAtIndex:i];
        NSNumber *value = [labelsAndValues objectAtIndex:i+1];
        [result setObject:value forKey:label];
    }
    return result;
}

static NSString* _JRPrivate_EnumToString(NSArray *labelsAndValues, NSString *enumTypeName, int enumValue) {
    NSString *result = [_JRPrivate_EnumByValue(labelsAndValues) objectForKey:[NSNumber numberWithInt:enumValue]];
    if (!result) {
        result = [NSString stringWithFormat:@"<unknown %@: %d>", enumTypeName, enumValue];
    }
    return result;
}

static BOOL _JRPrivate_EnumFromString(NSArray *labelsAndValues, NSString *enumLabel, NSInteger *enumValue) {
    NSNumber *value = [_JRPrivate_EnumByLabel(labelsAndValues) objectForKey:enumLabel];
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
static NSArray* _##ENUM_TYPENAME##LabelsAndValues() {	\
    NSString *enumString = @"" #ENUM_CONSTANTS; \
    return _JRPrivate_ParseEnumLabelsAndValuesFromString(enumString); \
} \
\
static NSDictionary* ENUM_TYPENAME##ByValue() {	\
    NSArray *labelsAndValues = _##ENUM_TYPENAME##LabelsAndValues();	\
    return _JRPrivate_EnumByValue(labelsAndValues); \
}	\
\
static NSDictionary* ENUM_TYPENAME##ByLabel() {	\
    NSArray *labelsAndValues = _##ENUM_TYPENAME##LabelsAndValues();	\
    return _JRPrivate_EnumByLabel(labelsAndValues); \
}	\
\
static NSString* ENUM_TYPENAME##ToString(int enumValue) {	\
    NSArray *labelsAndValues = _##ENUM_TYPENAME##LabelsAndValues();	\
    NSString *enumTypeName = @"" #ENUM_TYPENAME; \
    return _JRPrivate_EnumToString(labelsAndValues, enumTypeName, enumValue); \
}	\
\
static BOOL ENUM_TYPENAME##FromString(NSString *enumLabel, ENUM_TYPENAME *enumValue) {	\
    NSArray *labelsAndValues = _##ENUM_TYPENAME##LabelsAndValues();	\
    return _JRPrivate_EnumFromString(labelsAndValues, enumLabel, enumValue); \
}

//--

#define JREnumDeclare(ENUM_TYPENAME, ENUM_CONSTANTS...) \
    _Pragma("GCC warning \"JREnumDeclare is deprecated and should be replaced with JREnum.\"") \
    JREnum(ENUM_TYPENAME, ENUM_CONSTANTS)

//--


#define JREnumDefine(ENUM_TYPENAME) \
    _Pragma("GCC warning \"JREnumDefine is deprecated and should be removed.\"") \

//--
