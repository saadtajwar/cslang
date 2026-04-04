var a = 1 + 2 * 3;
print a;

var b;
b = a - 3;
print b;

if (b > 2 and a < 10) {
  print "if branch";
} else {
  print "else branch";
}

var i = 0;
while (i < 3) {
  print i;
  i = i + 1;
}

for (var j = 0; j < 3; j = j + 1) {
  print j;
}

fun add(x, y) {
  return x + y;
}

print add(3, 4);

fun makeCounter() {
  var count = 0;
  fun inc() {
    count = count + 1;
    return count;
  }
  return inc;
}

var counter = makeCounter();
print counter();
print counter();

class Animal {
  speak() {
    print "animal";
  }
}

class Dog < Animal {
  speak() {
    super.speak();
    print "dog";
  }

  identity() {
    print this;
  }
}

var d = Dog();
d.speak();
d.identity();

var truth = true or false and false;
print truth;

print !false;
print nil;