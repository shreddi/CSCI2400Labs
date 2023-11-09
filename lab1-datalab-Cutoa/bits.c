/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically and a negative shift value results in zero.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES, FOLLOW THE DIRECTIONS in README.md
 */


#endif
/* Copyright (C) 1991-2020 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */
/* wchar_t uses Unicode 10.0.0.  Version 10.0 of the Unicode Standard is
   synchronized with ISO/IEC 10646:2017, fifth edition, plus
   the following additions from Amendment 1 to the fifth edition:
   - 56 emoji characters
   - 285 hentaigana
   - 3 additional Zanabazar Square characters */
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
    //if both x and y are the same, l and r will both be false.
    //if x and y are different, either l or r will be true, and the other will be false.
    int l = x & ~y;
    int r = ~x & y;
    return ~(~l & ~r);
}

/* 
 * thirdBits - return word with every third bit (starting from the LSB) set to 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 1
 */
int thirdBits(void) {
    //Want to just return 0x49249249, but you have to make that number with little pieces because it is too big.
    int x = 0;
    x = x | 0x49; //Set what will be the MSB to 0100 1001
    x = x << 8; //shift by a byte (two hex places)
    x = x | 0x24;
    x = x << 8;
    x = x | 0x92;
    x = x << 8;
    x = x | 0x49; //Set the final, LSB to 0100 1001
    return x;
}

int x = 0;
x |= 0x99;
x << 8;
x |= 0x99;
// x = 0x00009999;
int temp = x;
x << 16;
x |= temp;




/* 
 * upperBits - pads n upper bits with 1's
 *  You may assume 0 <= n <= 32
 *  Example: upperBits(4) = 0xF0000000
 *  In general, a shift (left or right) by a negative number is undefined behavior.
 *  For this problem alone, you can assume that a left shift with a negative value is OK.
 *  and the resulting behavior is e.g. (0xf << -1 is 0). You may see an error raised
 *  at runt-time but you can ignore that error.
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 10
 *  Rating: 1
 */
int upperBits(int n) {
    //!!n accounts for the case where n is zero. If n is zero, this expression will yield zero and any shift operations will
    //pad no 1's, since the MSB is zero, once it is shifted left by 31.
    
    //~(~n+1) is just equal to n - 1. You negate n, then add 1, then negate the whole thing to get that value. You need this 
    //value to be n-1 because you already have the first 1 bit, and you need to shift in n-1 more. 
    return ((!!n) << 31) >> ~(~n + 1);
}
/* 
 * byteSwap - swaps the nth byte and the mth byte
 *  Examples: byteSwap(0x12345678, 1, 3) = 0x56341278
 *            byteSwap(0xDEADBEEF, 0, 2) = 0xDEEFBEAD
 *  You may assume that 0 <= n <= 3, 0 <= m <= 3
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 2
 */
int byteSwap(int x, int n, int m) {
    //convert n and m to bits instead of bytes.
    int nTimes8 = (n << 3); 
    int mTimes8 = (m << 3);
    
    //isolate the nth and mth byte.
    int nthByte = (x >> nTimes8) & 0xFF; // n << 3 is n*8, 8 bits in a byte, n bytes. 
    int mthByte = (x >> mTimes8) & 0xFF;
    
    //erase nth and mth. ~(0xFF << nTimes8) creates four zeros at byte n.
    int newX = x;
    newX = ~(0xFF << nTimes8) & newX;
    newX = ~(0xFF << mTimes8) & newX;
    
    //add m at n and vice versa
    newX = newX | (nthByte << mTimes8);
    newX = newX | (mthByte << nTimes8);
    
    return newX;
}
/* 
 * copyLSB - set all bits of result to least significant bit of x
 *   Example: copyLSB(5) = 0xFFFFFFFF, copyLSB(6) = 0x00000000
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int copyLSB(int x) {
  int newX = x << 31; //moves LSB to MSB with 31 trailing zeros.
  newX = newX >> 31; //shifts the MSB back to the LSB, now with all 1's or all 0's depending on what the original LSB was.
  return newX;
}
/* 
 * divpwr2 - Compute x/(2^n), for 0 <= n <= 30
 *  Round toward zero
 *   Examples: divpwr2(15,1) = 7, divpwr2(-33,4) = -2
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int divpwr2(int x, int n) {
//  If × is a positive number, x /(2^n) == x >> n;
//  If x is a negative number, x /(2^n) == (x + (1<<n) - 1)>>n (multiplying by 2^(-n)
//  -1=~0= oxffffffff
    int xIsNeg = (x >> 31); //-1 if x is neg, 0 if pos
    int y = (xIsNeg & ((1 << n) + ~0)); //0 if x is pos, ((1<<n) - 1) if x is neg
    return (x + y) >> n; //x >> n if x is pos, (x + (1<<n) - 1) >> n if x is neg
}
/* 
 * isEqual - return 1 if x == y, and 0 otherwise 
 *   Examples: isEqual(5,5) = 1, isEqual(4,5) = 0
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int isEqual(int x, int y) {
    //xor returns 0 if two bits are the same. So, if all the bits are the same, xor'ing the bits returns all zero, so     negating this returns 1.
  return !(x^y);
}
/* 
 * isLess - if x < y  then return 1, else return 0 
 *   Example: isLess(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLess(int x, int y) {
//  There are 2 cases that can lead to x < y
//     x < 0 and y >= 0 (case a)
//     × and y have the same sign, and (× - y) is negative (case b)
//  • Use 2's complement for subtraction (e.g., x-y=x+~y+1)
//  • XOR operation returns O when two numbers are identical
    
    int diffSigns = (x >> 31) ^ (y >> 31); //-1 if x and y have different signs
    int a = diffSigns & (x >> 31); //-1 if x and y have different signs and x < 0
    int xMinusY = (x + ~y + 1); //~y = -y - 1
    int sumIsNeg = xMinusY >> 31; //-1 if x - y is neg, 0 if x - y is pos
    int b = ~diffSigns & sumIsNeg; //-1 if the signs are the same and x - y is neg
//     printf("Bruh\\n");
//     printf("%d\\n",diffSigns);
//     printf("%d\\n",a);
//     printf("%d\\n",xMinusY);
//     printf("%d\\n",sumIsNeg);
//     printf("%d\\n",b);
    return (a | b) & 1; //the & 1 isolates the LSB, since a and b are 0xFFFFFFFF if true.
}
/* 
 * rempwr2 - Compute x%(2^n), for 0 <= n <= 30
 *   Negative arguments should yield negative remainders
 *   Examples: rempwr2(15,2) = 3, rempwr2(-35,3) = -3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int rempwr2(int x, int n) {
    //rempwr2(x, n) = x - (divpwr2(x, n) << n)
    
    int xIsNeg = (x >> 31); //-1 if x is neg, 0 if pos
    int y = (xIsNeg & ((1 << n) + ~0)); //0 if x is pos, ((1<<n) - 1) if x is neg
    int z = (x + y) >> n; //x >> n if x is pos, (x + (1<<n) - 1) >> n if x is neg
    return x + ~(z << n) + 1; //subtraction is negation plus 1.
}
/*
 * satMul2 - multiplies by 2, saturating to Tmin or Tmax if overflow
 *   Examples: satMul2(0x30000000) = 0x60000000
 *             satMul2(0x40000000) = 0x7FFFFFFF (saturate to TMax)
 *             satMul2(0x60000000) = 0x80000000 (saturate to TMin)
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int satMul2(int x) {
    int xDoubled = x << 1; //x times 2
    int isOverflow = (x^(x+x)) >> 31; //-1 if overflow, 0 otherwise. Checks for overflow if MSB changes.
    int newX = ~isOverflow & xDoubled; //2*x if not overflow, 0 otherwise
    //int isNeg = (x >> 31); //-1 if x is neg, 0 otherwise
    int toAdd = (1 << 31) + ((x+x)>>31); //Tmax or Tmin depending on negative or positive overflow
//     printf("Bruh\\n");
//     printf("%d\\n",xDoubled);
//     printf("%d\\n",isOverflow);
//     printf("%d\\n",newX);
//     printf("%d\\n",toAdd);
    return newX + (isOverflow & toAdd); 
    
}
/* 
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int bang(int x) {
  int negX = (~x+1); //makes x negative of itself. If x is zero, then it will still be zero.
  return ((negX | x) >> 31)+1; //if x was zero to begin with, you add 1 to 0. If it was anything besides zero, you add 1 to-1
}
/* reverseBits - reverse the bits in a 32-bit integer,
              i.e. b0 swaps with b31, b1 with b30, etc
 *  Examples: reverseBits(0x11111111) = 0x88888888
 *            reverseBits(0xdeadbeef) = 0xf77db57b
 *            reverseBits(0x88888888) = 0x11111111
 *            reverseBits(0)  = 0
 *            reverseBits(-1) = -1
 *            reverseBits(0x9) = 0x90000000
 *  Legal ops: ! ~ & ^ | + << >> and unsigned int type
 *  Max ops: 90
 *  Rating: 4
 */
int reverseBits(int x) {
    int newX;
    
    //swap adjacent bits
    //0x55555555 = 0b01010101...
    int temp = x & 0x55555555;
    temp = temp << 1;
    newX = (x >> 1) & (0x55555555);
    newX = newX | temp;
    //printf("%d\\n",newX);
    
    //swap adjacent pairs of bits
    //0x33333333 = 0b00110011...
    temp = newX & 0x33333333;
    temp = temp << 2;
    newX = (newX >> 2) & (0x33333333);
    newX = newX | temp;
    //printf("%d\\n",newX);
    
    //swap adjacent nibbles
    //0b0000111100001111...
    temp = newX & 0x0F0F0F0F;
    temp = temp << 4;
    newX = (newX >> 4) & (0x0F0F0F0F);
    newX = newX | temp;
    //printf("%d\\n",newX);
    
    //swap adjacent bytes
    //0b0000000011111111...
    temp = newX & 0x00FF00FF;
    temp = temp << 8;
    newX = (newX >> 8) & (0x00FF00FF);
    newX = newX | temp;
    newX = newX | temp;
    //printf("%d\\n",newX);
    
    //swap adjacent words
    temp = newX & 0x0000FFFF;
    temp = temp << 16;
    newX = (newX >> 16) & (0x0000FFFF);
    newX = newX | temp;
    newX = newX | temp;
    //printf("%d\\n",newX);
    
    
    return newX;
}
/*
 * ilog2 - return floor(log base 2 of x), where x > 0
 *   Example: ilog2(16) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 90
 *   Rating: 4
 */
int ilog2(int x) {
    int ans = 0;
    
    int temp = !!(x >> 16) << 4; //16 if 1 in left half, 0 otherwise.
    ans += temp;
    
    /*  if 1 was in the left half, check the left quarter, otherwise check the second least significant quarter.
        adds 8 to result if there is a 1 in the specified quarter.
        should check left quarter if already at 16, since bits in the right half won't make it greater than 16.
    */
    temp = !!(x >> (8 + ans)) << 3; //left shifting the bit by 3 makes it 8.
    ans += temp;
    
    temp = !!(x>> (4 + ans)) << 2; //adds 4 if there is a one in the specified nibble.
    ans += temp;
    
    temp = !!(x>> (2 + ans)) << 1; //adds 2 if there is a one in the specified duo.
    ans += temp;
    
    temp = !!(x >> (1 + ans)); //adds 1 if there is a 1 at the specified bit.
    ans += temp;
    
    return ans;
}

/*
 * Extra credit
 */
/* 
 * float_half - Return bit-level equivalent of expression 0.5*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_half(unsigned uf) {
    int s = uf & 0x80000000; //sign
    int e = (uf & 0x7F800000); //exponent
    if(e == 0x7F800000){ //if NAN
        //don't change uf, return original
        printf("%d\n",1111111);
    }
    else if(!(e>>24)){ //exponent is 0 and number can't be divided by subtracting from exponent. Instead, shift significand.
        int round = 0; 
        if(uf & 3 == 3)
            round = 1;
        printf("%d\n",9999999);
        printf("%d\n",uf);
        printf("%d\n",uf & 3);
//determines if rounding is needed
        uf >>= 1; //divide by 2;
        //printf("%d\\n",uf);
        //erase offset sign and set it to be what it originally was.
        uf &= 0xBFFFFFFF;
        //printf("%d\\n",uf);
        uf |= s;
        uf += round;
        //printf("%d\\n",uf);
//         printf("%d\n",22222);
//         printf("%d\n",round);
    }
    else{
        e >>= 23;
        e -= 1; //subtract 1 from exponent, dividing float by 2
        e <<= 23;
        //erase old exp, add new
        uf &= ~0x7F800000;
        uf |= e;
        printf("%d\n",3);
    }
    //printf("%d\\n",uf);
    return uf;
}
/* 
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_i2f(int x) {
  return 2;
}
