#!/usr/bin/python3

import sys

TYPE = "bdd_ptr "

def writeln(f, s):
  f.write(s)
  f.write('\n')
  f.flush()

def writeheader(f, circuit):
  writeln(f, "#include <bdd.h>")
  writeln(f, "#include <iostream>")
  writeln(f, "int main() {")
  numnodes = ''.join(circuit).count("INPUT")+2
  writeln(f, "bdd_init({ut_size},{num},{numnodes});".format(numnodes=numnodes,
                                                            ut_size='1<<20',
                                                            num=0))

def writefooter(f):
  writeln(f, "return 0;")
  writeln(f, "}")

def emit(cir, f=sys.stdout):
  writeheader(f, cir)

  i = 0
  linenum = 0
  outs = set()

  for line in cir:
    line = line.strip()
    if line.startswith('#'): continue

    if line.startswith('INPUT'):
      name = line.split('(')[1][:-1].strip()
      emitithvar(name, i, f)
      i += 1
      continue

    if line.startswith('OUTPUT'):
      name = line.split('(')[1][:-1].strip()
      outs.add(name)
      continue

    line = line.split('=')
    if len(line) != 2:
      continue
    var, expr = line
    var = var.strip()
    expr = expr.strip()

    gate = expr.split('(')[0].strip()
    ops = expr.split('(')[1][:-1].split(',')

    writeln(f, 'std::cout << {line} << std::endl;'.format(line=linenum))
    linenum += 1

    if gate == 'BUFF':
      emitbuff(var, ops[0], f)
      continue

    emitsource(var, gate, ops, f)
  writefooter(f)

def emitbuff(var, op, f):
  writeln(f, '{type}{var} = {op};'.format(type=TYPE, var='v'+var, op='v'+op))

def emitithvar(name, i, f):
  writeln(f, '{type}{var} = ithvar({i});'.format(type=TYPE, var='v'+name, i=i))

def emitcall(var, gate, ops, f):
  writeln(f, '{type}{var} = {fn}({ops});'.format(type=TYPE,
                var='v'+var,
                fn='bdd_'+gate.lower(),
                ops=','.join(['v'+o.strip() for o in ops])))

def emitsource(var, gate, ops, f):
  if len(ops) <= 2:
    emitcall(var, gate, ops, f)
    return

  def formatvar(var, depth, i):
    return '{v}_{d}_{i}'.format(v=var, d=depth, i=i)

  last = None
  if len(ops)%2 == 1:
    last = ops[-1]
    ops = ops[:-1]

  step = 2
  depth = 1
  while step < len(ops):
    for i in range(0, len(ops), step):
      if depth == 1:
        op1 = ops[i]
        op2 = ops[i+1]
      else:
        op1 = formatvar(var, depth-1, i)
        op2 = formatvar(var, depth-1, i+1*depth)
      emitcall(formatvar(var, depth, i), gate, [op1, op2], f)
    step *= 2
    depth += 1

  if last == None:
    op1 = formatvar(var, depth-1, 0)
    op2 = formatvar(var, depth-1, 0+1*step//2)
    emitcall(var, gate, [op1, op2], f)
  else:
    op1 = formatvar(var, depth-1, 0) if depth-1 > 0 else ops[0]
    op2 = formatvar(var, depth-1, 0+1*step//2) if depth-1 > 0 else ops[1]
    tmp = formatvar(var, depth, 0)
    emitcall(tmp, gate, [op1, op2], f)
    emitcall(var, gate, [tmp, last], f)
    

def main():
  if len(sys.argv) != 2:
    print("Usage: {} circuit.bench".format(sys.argv[0]))
    return
  with open(sys.argv[1]) as f:
    circuit = f.readlines()
  emit(circuit)

if __name__ == '__main__':
  main()
