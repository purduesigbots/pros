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
			venv.run 'pip3 install pros-cli'
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
		tool 'arm-gcc'
		venv.run 'pros make template'
	}
}
