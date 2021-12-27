
# BadCrypto
 Is a **non**-cryptographically secure file encryption program / test.

## Working parts
To hash the plain text password into something the number generator can use, a Pearson hash was used. This hash value becomes the sudo random number generator seed. The sudo random number generator is a linear feedback shift register. The numbers generated from the LFSR get xor-ed with the plain text to create our new cipher text.

# Major security issues
The LFSR has a period of 256, meaning, it would only take an adversary 256 tries to brute force our encrypted file, assuming they knew the encryption mechanism. Even if they didn't, encrypting a data block of 0's would quickly reveal a pattern. Not to mention that the LFSR was originally a hardware based crypto number generator that dates back to the 60's. The bottom line is, its not a good number generator for crypto applications.

I'm a bit miss informed when it comes to hashing algorithms, but the general  consensus is that the Pearson hash is not a secure hash.

## Take away 
This was just a fun coding exercise in algorithmic implementation / security. This is by no means a **good** crypto program.

The working parts were mainly implemented for speed. The above is not very taxing in terms of computation. So I could see this being used in a very low power system that requires very basic obfuscation. 


### Borrowed works maintain original license.