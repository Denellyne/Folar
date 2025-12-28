# Lambdas Syntax

## Declaration

Lambdas can be declared with or without name, but only if passed as an argument as an unnamed lambda as no way of being called otherwise.

Lambdas that are represented by the return of the expression are able to be declared without the brackets.

The arguments of a lambda can be given any name given they dont share a name with any other variable inside that lambda being another argument or a capture or the lambda name itself.

Examples:

```hs
lambda cond[] :: int8 -> int8 -> boolean 
    cond x y => x < y;

lambda sumWithX[] :: int8 -> lambda (int8 -> int8) {
    sumWithX x => lambda[x] :: int8 -> int8 {
      y => x + y;
    }
  }  
  
lambda sumWith5[x : int8 = 5] :: lambda (int8 -> int8) {
    sumWith5 => lambda[x] :: int8 -> int8 {
      y => x + y;
    }
  }

  lambda sumLoop[] :: mut int8 -> int8 -> lambda(int8 -> int8 -> boolean) -> int8 {
    sumLoop x y condition => 
      while(condition(x,y)){
        x = sumWith5(x);
      }
  }
```

## Common type

These are the most common types of lambdas, they don't capture anything, they return trivial types and are basic in their signature

Examples:

```hs
lambda cond[] :: int8 -> int8 -> boolean 
    cond x y => x < y;

lambda sum[] :: int8 -> int8 
    sum x => x + 5;
  
lambda foo[] :: boolean 
  foo => true;

lambda incVec1[] :: &int8[] -> void
  incVec1 nums => 
    for num in nums 
      num++;

```

## Lambdas returning lambdas

These lambdas return a new lambda, this is useful if you're using it to bind an argument to a previously defined lambda

Examples:

```hs
lambda sumWithX[] :: int8 -> lambda (int8 -> int8) {
    sumWithX x => lambda[x] :: int8 -> int8 {
      y => x + y;
    }
  }

  lambda sumWith5[] :: lambda ( int8 -> int8 )  {
    sumWith5 => sumWithX(5);
  }

```
> [!NOTE]
> `sumWith5` can now be called, and the number `5` will substitute the `x` variable in the `sumWithX` lambda.
  
**Formal Definition:**

$$sumWithX = \lambda x y . (x + y)$$

$$sumWith5 = \lambda x y . (x + y) [x \to 5] = \lambda y . (5 + y)$$
  
## Lambda capture

Lambda captures are special because they allow you to capture outside variables, create a variable only for the scope of that lambda, or even take a reference to an outside variable

This is possible because a lambda is bound to the scope of the function that defines it, as such, anything declared before the lambda is able to be captured.

Example:

```hs
fn foo(items : int8[]) -> boolean {
  let vec : int8[] = {1,3,123,5,31,4,6,23,55,13,5,46,2};
  let mut x : int8;
  lambda is55[&x] :: boolean
    is55 => x == 55;

  for num in vec{
    x = num;
    if(is55(x)) break;
  }

  return x == 55;
}

```

> [!IMPORTANT]
> 
> **Initialization Safety**
> 
> The lambda `is55` will throw a **compile-time error** if the variable `x` is not initialized before the first call. This applies to any use of mutable variables.

> [!WARNING]
> 
> Had the lambda captured by **value** instead of **reference**, it would throw a compile-time error immediately, as this would be classified as using a variable before initialization.



  

 
