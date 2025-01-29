pipeline {

  agent any

  stages {
    stage ("Build") {
      steps {
        sh 'ninja'
      }
    }

    stage ("Test") {
      steps {
        sh './build/pathfinder_tests -t -c'
      }
    }
  }

  post { 
    always { 
      cleanWs()
    }
  }
}
