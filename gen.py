import glob, subprocess as subp

src = {
    'cc': glob.glob('src/**/*.c', recursive=True),
    'as': glob.glob('src/**/*.s', recursive=True)
}

with open('build.ninja', 'w') as fout:
    fout.write('# generated automatically, edit gen.py instead\n')
    fout.write('include rules.ninja')
    fout.write('\n')

    ofs = []
    for k, fs in src.items():
        for f in fs:
            if '~' in f: continue
            if '#' in f: continue
            fout.write('build build/' + f + '.o: ' + k + ' ' + f + '\n')
            ofs.append('build/' + f + '.o')
    
    fout.write('\n')
    fout.write('build boot/kernel.elf: ld ' + ' '.join(ofs) + '\n')
    fout.write('build dist/out.iso: mkbootimg config.json | boot/kernel.elf\n')
    fout.write('default dist/out.iso\n')

with open('compile_commands.json', 'w') as fout:
    fout.write(subp.getoutput('ninja -t compdb'))
    fout.write('\n')
