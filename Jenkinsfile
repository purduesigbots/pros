stage('Build') {
	node('lin&&prostools') {
		checkout scm
		sh '''
			pip3 install --no-cache-dir git+git://github.com/purduesigbots/pros-cli.git@master
			pros make template
		   '''
		archiveArtifacts artifacts: 'kernel-template.zip', onlyIfSuccessful: true
	}
}

