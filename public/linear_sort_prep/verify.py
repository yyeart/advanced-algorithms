import pathlib
import random
import subprocess

root = pathlib.Path(__file__).resolve().parent
rng = random.Random(20261002)
names = ['01_counting', '02_radix_numbers', '03_radix_records', '04_radix_strings', '05_bucket']
for name in names:
    subprocess.run(['clang++', '-std=c++17', '-Wall', '-Wextra', '-pedantic', '-fsanitize=address,undefined', str(root / (name + '.cpp')), '-o', '/private/tmp/' + name], check=True)

def run(name, data):
    p = subprocess.run(['/private/tmp/' + name], input=data, text=True, capture_output=True, check=True)
    assert not p.stderr, p.stderr
    return p.stdout.split()

counts = dict.fromkeys(names, 0)
for a in [[], [0], [4]*5, [9,0,9,1], list(range(20)), list(range(19,-1,-1))] + [[rng.randrange(101) for _ in range(rng.randrange(80))] for _ in range(100)]:
    got = list(map(int, run('01_counting', f'{len(a)} 100\n' + ' '.join(map(str,a)))))
    assert got == sorted(a)
    counts['01_counting'] += 1

for a in [[], [0], [0]*8, [2147483647,0,1,2147483646], [100,10,1,0], [21,11,12]] + [[rng.randrange(2147483648) for _ in range(rng.randrange(80))] for _ in range(100)]:
    got = list(map(int, run('02_radix_numbers', f'{len(a)}\n' + ' '.join(map(str,a)))))
    assert got == sorted(a)
    counts['02_radix_numbers'] += 1

for keys in [[], [0], [-1000000000,1000000000,0,-1,1], [2,1,2,1,2], [-1000000000]*5] + [[rng.randrange(-20,21) for _ in range(rng.randrange(80))] for _ in range(100)]:
    a = [(k, f'id{i}') for i,k in enumerate(keys)]
    out = run('03_radix_records', f'{len(a)}\n' + '\n'.join(f'{k} {v}' for k,v in a))
    got = [(int(out[i]),out[i+1]) for i in range(0,len(out),2)]
    assert got == sorted(a,key=lambda x:x[0])
    counts['03_radix_records'] += 1

for a in [[], ['a'], ['ab','a','aa','b','aaa','z'], ['cat','cab','car','cat'], ['z']*5] + [[''.join(rng.choice('abcxyz') for _ in range(rng.randrange(1,15))) for _ in range(rng.randrange(80))] for _ in range(100)]:
    got = run('04_radix_strings', f'{len(a)}\n' + '\n'.join(a))
    assert got == sorted(a)
    counts['04_radix_strings'] += 1

for a in [[], [0.0], [0.0,0.9999999999999999,0.5,0.0], [0.31,0.30,0.32,0.30], [0.5]*10] + [[rng.random() for _ in range(rng.randrange(80))] for _ in range(100)]:
    got = list(map(float, run('05_bucket', f'{len(a)}\n' + ' '.join(map(repr,a)))))
    assert got == sorted(a)
    counts['05_bucket'] += 1
print(counts)
print('Total:', sum(counts.values()), 'cases passed; C++17, ASan and UBSan enabled.')
