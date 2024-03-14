pipeline {
    agent any

    stages {
        stage('Build') {
            steps {
                sh 'echo "Begin of pipeline"'
            }
        }
        stage('Test'){
            steps {
                sh 'echo "Second stage"'
            }
        }
        stage('Deploy') {
            steps {
                sh 'echo "Third stage"'
            }
        }
    }
}