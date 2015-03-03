// JREnum.h semver:1.1
//   Original implementation by Benedict Cohen: http://benedictcohen.co.uk
//   Copyright (c) 2012-2014 Jonathan 'Wolf' Rentzsch: http://rentzsch.com
//   Some rights reserved: http://opensource.org/licenses/mit
//   https://github.com/rentzsch/JREnum

#pragma mark - Private functions (called by functions defined in macros)
/**
 Parses enumString into an array of interleaved labels and keys.
 
 TODO: Document restrictions on key. The follow, for example, will have strange results:
     1. JREnumValue = NSNotFound
     2. JREnumNewValue = 0, JRValueOldEnum = JREnumNewValue,
     3. ArfVoid = 0, Arf1, Arf2 = (Arf1 * (2 + ArfVoid)),
 It's possible to improve the function so that it can parse cases 2 and 3 but not case 1.

 @param enumString a string of comma delimitted labels and optional keys that form a valid C enum body.

 @return an array of interleaved labels and keys.
 */
static NSArray* _JRPrivate_ParseEnumLabelsAndValuesFromString(NSString *enumString) {
    NSString *normalized = [[enumString componentsSeparatedByCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]] componentsJoinedByString:@""];
    if ([normalized hasSuffix:@","]) {
        normalized = [normalized substringToIndex:[normalized length]-1];
    }
    NSArray *stringPairs = [normalized componentsSeparatedByString:@","];
    NSMutableArray *labelsAndValues = [NSMutableArray arrayWithCapacity:[stringPairs count]];
    NSInteger nextDefaultValue = 0;
    for (NSString *stringPair in stringPairs) {
        NSArray *labelAndValueString = [stringPair componentsSeparatedByString:@"="];
        NSString *label = [labelAndValueString objectAtIndex:0];
        NSString *valueString = [labelAndValueString count] > 1 ? [labelAndValueString objectAtIndex:1] : nil;
        NSInteger value;
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

/**
 Converts a labels & values array into a dictionary. If multiple labels have the same value then the dictionary will contain the last matching label.

 @param labelsAndValues An array of interleaved labels and values.

 @return a dictionary were the keys are the enum values and the values are the enum labels.
 */
static NSDictionary* _JRPrivate_EnumByValue(NSArray *labelsAndValues) {
    NSMutableDictionary *result = [NSMutableDictionary dictionaryWithCapacity:[labelsAndValues count] / 2];
    for (NSUInteger i = 0; i < [labelsAndValues count]; i += 2) {
        NSString *label = [labelsAndValues objectAtIndex:i];
        NSNumber *value = [labelsAndValues objectAtIndex:i+1];
        [result setObject:label forKey:value];
    }
    return result;
}

/**
 Converts a labels & values array into a dictionary.

 @param labelsAndValues An array of interleaved labels and values.

 @return a dictionary were the keys are the enum labels and the values and the enum values.
 */
static NSDictionary* _JRPrivate_EnumByLabel(NSArray *labelsAndValues) {
    NSMutableDictionary *result = [NSMutableDictionary dictionaryWithCapacity:[labelsAndValues count] / 2];
    for (NSUInteger i = 0; i < [labelsAndValues count]; i += 2) {
        NSString *label = [labelsAndValues objectAtIndex:i];
        NSNumber *value = [labelsAndValues objectAtIndex:i+1];
        [result setObject:value forKey:label];
    }
    return result;
}

/**
 Returns an enum label as a string for the given enum value. If multiple labels have the same value then the result will be the label that was defined latest in the enum.

 @param labelsAndValues An array of interleaved labels and values.
 @param enumTypeName    The name of the enum.
 @param enumValue       The value to lookup.

 @return The label for enumValue.
 */
static NSString* _JRPrivate_EnumToString(NSArray *labelsAndValues, NSString *enumTypeName, NSInteger enumValue) {
    NSString *result = [_JRPrivate_EnumByValue(labelsAndValues) objectForKey:[NSNumber numberWithInt:enumValue]];
    if (!result) {
        result = [NSString stringWithFormat:@"<unknown %@: %d>", enumTypeName, enumValue];
    }
    return result;
}

/**
 Returns by reference the the value for the specified label.

 @param labelsAndValues An array of interleaved labels and values.
 @param enumLabel       The label to lookup.
 @param enumValue       On return contains the value for the specified label.

 @return YES on success otherwise NO.
 */
static BOOL _JRPrivate_EnumFromString(NSArray *labelsAndValues, NSString *enumLabel, NSInteger *enumValue) __attribute__((nonnull(3)));
static BOOL _JRPrivate_EnumFromString(NSArray *labelsAndValues, NSString *enumLabel, NSInteger *enumValue) {
    NSNumber *value = [_JRPrivate_EnumByLabel(labelsAndValues) objectForKey:enumLabel];
    if (value) {
        *enumValue = [value integerValue];
        return YES;
    } else {
        return NO;
    }
}



#pragma mark - JREnum macro
//--

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
static NSString* ENUM_TYPENAME##ToString(NSInteger enumValue) {	\
    NSArray *labelsAndValues = _##ENUM_TYPENAME##LabelsAndValues();	\
    NSString *enumTypeName = @"" #ENUM_TYPENAME; \
    return _JRPrivate_EnumToString(labelsAndValues, enumTypeName, enumValue); \
}	\
\
static BOOL ENUM_TYPENAME##FromString(NSString *enumLabel, ENUM_TYPENAME *enumValue) __attribute__((nonnull(2))); \
static BOOL ENUM_TYPENAME##FromString(NSString *enumLabel, ENUM_TYPENAME *enumValue) {	\
    NSArray *labelsAndValues = _##ENUM_TYPENAME##LabelsAndValues();	\
    return _JRPrivate_EnumFromString(labelsAndValues, enumLabel, enumValue); \
}



#pragma mark - Deprecated macros
//--

#define JREnumDeclare(ENUM_TYPENAME, ENUM_CONSTANTS...) \
    _Pragma("GCC warning \"JREnumDeclare is deprecated and should be replaced with JREnum.\"") \
    JREnum(ENUM_TYPENAME, ENUM_CONSTANTS)

//--

#define JREnumDefine(ENUM_TYPENAME) \
    _Pragma("GCC warning \"JREnumDefine is deprecated and should be removed.\"") \

//--
