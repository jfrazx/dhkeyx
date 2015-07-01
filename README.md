
# DHKeyX

An implementation of the Diffie–Hellman key exchange for Ruby.

This is a fork of: https://rubygems.org/gems/dhkeyexchange for Ruby >= 1.9.2.

## INSTALLATION

  $[sudo] gem install dhkeyx

  OSX 10.7 and higher will get deprecation warnings due to Apples attempt to move away from OpenSSL

## USAGE

```
  require 'dhkeyx'

  #DHKey.new([base], [prime], [private_key])

  bob = DHKey.new

  alice = DHKey.new(bob.base, bob.prime)

  bob.their_public_key = alice.x
  alice.y              = bob.my_public_key

  bob.shared_key == alice.k
  => true

  bob.private_key == alice.a
  => false

```

### METHODS => ALIAS
  'base=' => 'g='
  
  'prime=' => 'p='
  
  'private_key=' => 'a='
  
  'their_public_key=' => 'y='

   ----

  'base' => 'g'
  
  'prime' => 'p'
  
  'private_key' => 'a'
  
  'my_public_key' => 'x'
  
  'their_public_key' => 'y'
  
  'shared_key' => 'k' 

