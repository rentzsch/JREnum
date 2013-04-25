# JREnum

Original [idea](https://twitter.com/benedictC/status/277867522869571584) and [implementation](https://gist.github.com/4246759) by [Benedict Cohen](http://benedictcohen.co.uk).

JREnum is a macro that automates creation of functions that blast enums from boring primitive compile-time-land to the fun-filled party environment of runtime.

Let's use a concrete example. Instead of writing:

	typedef enum {
	    Stream_Disconnected,
	    Stream_Connecting,
	    Stream_Connected,
	    Stream_Disconnecting
	}   StreamState;

write:

	JREnum(StreamState,
	       Stream_Disconnected,
	       Stream_Connecting,
	       Stream_Connected,
	       Stream_Disconnecting);

This will generate the previous `typedef enum` and will also generate a corresponding suite of functions:

**NSString\* StreamStateToString(int value)**

Given a value, will return the enum's string representation. For example `StreamStateToString(2)` would return `@"Stream_Connecting"`.

When confronted with values not defined in the enumeration, this function will return a placeholder string explaining the situation. For example `StreamStateToString(2000)` would return `@"<unknown StreamState: 2000>"`.

**BOOL StreamStateFromString(NSString \*enumLabel, StreamState \*enumValue)**

Attempts to return the enum's `int` value given its label. For example `StreamStateFromString(@"Stream_Disconnecting", &value)` would return `YES` and set `value` to `3`.

This function returns `NO` if the label for the enum type is unknown. For example `StreamStateFromString(@"lackadaisical", &value)` would return `NO` and leave `value` untouched.

**NSDictionary\* StreamStateByValue()**

Returns a dictionary whose keys are the enum's values. Used by StreamStateToString().

When enums have multiple overlapping values, the current implementation exhibits last-write-wins behavior.

**NSDictionary\* StreamStateByLabel()** 

Returns a dictionary whose keys are the enum's labels. Used by StreamStateFromString(). This is the function you want if you wish to enumerate an enum's labels and values at runtime.

## Split Header / Source Files

JREnum() is fine for when you have an enum that lives solely in an .m file. But if you're exposing an enum in a header file, you'll have to use the alternate macros. In your .h, use JREnumDeclare():

	JREnumDeclare(StreamState,
	              Stream_Disconnected,
	              Stream_Connecting,
	              Stream_Connected,
	              Stream_Disconnecting);

And then use JREnumDefine() in your .m:

	JREnumDefine(StreamState);

## Explicit Values

You can also explicitly define enum integer values:

	JREnum(StreamState,
	       Stream_Disconnected = 42,
	       Stream_Connecting,
	       Stream_Connected,
	       Stream_Disconnecting);

In the above scenario, `Stream_Disconnected`'s value will be `42`, `Stream_Connecting`'s will be `43` and so on.

## Bitwise enums:  (NS_OPTIONS)
    
Same syntax as above…  but exchange with 

`JROptions(TYPE_NAME,…)` OR `JROptionsDeclare(TYPE_NAME,…)` + `JROptionsDefine(TYPE_NAME)`.
	JROptions (Align,   AlignLeft    	 	= 0x00000001,
	       	      		AlignRight  	  	= 0x00000010,
				    	AlignTop		   	= 0x00000100,
				    	AlignBottom		    = 0x00001000,
				    	AlignTopLeft	   	= 0x00000101,
	    				AlignBottomLeft 	= 0x00001001,
				    	AlignTopRight	 	= 0x00000110,
	    				AlignBottomRight	= 0x00001010
     );

	NSLog(@"%@",AlignByValue()); ->		{	256 = AlignTop;
							    			4096 = AlignBottom;	
									    	257 = AlignTopLeft;
									    	1 = AlignLeft;
									    	4097 = AlignBottomLeft;
									    	16 = AlignRight;
									    	272 = AlignTopRight;
									    	4112 = AlignBottomRight;
										}
	

Combinations, like | Apple | does...

	Align botRight 		  = AlignBottomRight;
	Align botRightBitWise = AlignBottom	| AlignRight;

	NSLog(@"Are They The Same: %@", (botRightBitWise == botRight) ? @"YES" : @"NO"); ->
> Are They The Same: YES

But better, because you can go to-and-fro string values

	NSLog(@"How is that combo aligned? %@", AlignToString(botRightBitWise)); ->

> How is that combo aligned? AlignBottomRight


JREnum currently <s>only supports integer explicit values (bit shifts and masks won't work).</s> Patches welcome.

## Version History

### v1.0: Apr 09 2013

* Minor bug fix from 0.2.

### v0.2: Dec 10 2012

* [NEW] Generalized to support bidirectional enum label/value lookup and full runtime access to lookup dictionary.
* [NEW] Add passing tests.
* [NEW] Write this README.

### v0.1: Dec 9 2012

* [NEW] Devised way to allow split declaration/definition macros to allow use in header/source files.

### Prelude: Dec 8 2012

* I [publish a Ruby script](http://rentzsch.tumblr.com/post/37512716957/enum-nsstring) to automate the generation of NSStrings from an enum declaration.
* [Benedict Cohen](http://benedictcohen.co.uk) [tweets](https://twitter.com/benedictC/status/277867522869571584) his [macro idea and implementation](https://gist.github.com/4246759) to me.
