stage('Build') {
	def build_ver = '2.11.0'
	node {
		def venv = new edu.purdue.pros.venv()
		stage('Clean') {
			if(isUnix()) {
				sh 'git init'
				sh 'git clean -x -d -f'
			} else {
				bat 'git init'
				bat 'git clean -x -d -f'
			}
		}
		stage('PROS CLI') {
			tool 'python3'
			if(isUnix()) {
			    sh 'sudo apt-get install -y python3-pip'
			}
			venv.create_virtualenv()
			venv.run 'pip3 install --no-cache-dir git+git://github.com/purduesigbots/pros-cli.git@master', sudo = true
		}
		stage('Clone') {
			checkout scm
			if(isUnix()) {
				sh 'git describe --tags > version'
			} else {
				bat 'git describe --tags > version'
			}
			build_ver = readFile 'version'
			echo "Build PROS at version ${build_ver}"
		}
		toolchain_dir = tool 'arm-gcc'
		withEnv(["PROS_TOOLCHAIN=${toolchain_dir}"]) {
		    venv.run 'pros conduct first-run --use-defaults --no-download --no-force'
		    venv.run 'pros make template'
		}
		zip archive: true, dir: 'template', glob: '', zipFile: "kernel-template.zip"
	}
}

