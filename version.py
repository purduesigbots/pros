from __future__ import print_function
import subprocess
import io
import os

try:
    v = subprocess.check_output(['git', 'describe', '--dirty', '--abbrev']).decode().strip()
    if '-' in v:
        bv = v[:v.index('-')]
        bv = bv[:bv.rindex('.') + 1] + str(int(bv[bv.rindex('.') + 1:]) + 1)
        if os.environ.get('SYSTEM_PULLREQUEST_PULLREQUESTNUMBER', None):  # for Azure Pipelines PR builder
            sempre = 'pr{}'.format(os.environ.get('SYSTEM_PULLREQUEST_PULLREQUESTNUMBER'))
            build = os.environ.get('BUILD_BUILDID')
        else:
            sempre = 'dirty' if v.endswith('-dirty') else 'commit'
            # pippre = 'alpha' if v.endswith('-dirty') else 'pre'
            build = subprocess.check_output(['git', 'rev-parse', '--short', 'HEAD']).decode().strip()
            number_since = subprocess.check_output(['git', 'rev-list', v[:v.index('-')] + '..HEAD', '--count']).decode().strip()
            build = "{}.{}".format(number_since, build)
        semver = bv + '-' + sempre + '.' + build
    else:
        semver = v

    with open('version', 'w') as f:
        print('Semantic version is ' + semver)
        f.write(semver)

    assert semver.count('.') >= 2
    major, minor, patch = semver.split('.', 2)
    patch = patch.split('-', 1)[0]
    with io.open('include/pros/version.h', 'r', encoding='ascii') as file:
        data = file.readlines()
    for i, line in enumerate(data):
        if '#define PROS_VERSION_MAJOR' in line:
            data[i] = u'#define PROS_VERSION_MAJOR {}\n'.format(major)
        if '#define PROS_VERSION_MINOR' in line:
            data[i] = u'#define PROS_VERSION_MINOR {}\n'.format(minor)
        if '#define PROS_VERSION_PATCH' in line:
            data[i] = u'#define PROS_VERSION_PATCH {}\n'.format(patch)
        if '#define PROS_VERSION_STRING ' in line:
            data[i] = u'#define PROS_VERSION_STRING "{}"\n'.format(semver)
    with io.open('include/pros/version.h', 'w', newline='\n', encoding='ascii') as file:
        file.writelines(data)

except subprocess.CalledProcessError as e:
    print('Error calling git')
