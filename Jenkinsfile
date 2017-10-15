stage('Build') {
	node('linux&&prostools') {
		checkout scm
		sh '''
			sudo pip3 install vex
			vex -mr kernel ./build.sh
		   '''
		archiveArtifacts artifacts: 'kernel-template.zip', onlyIfSuccessful: true
	}
}
