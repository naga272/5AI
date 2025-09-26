# First Git Exercise

## Chapter 1: Creation of the Repository

We are going to see how to create our Repository. We will break this action in three distinct phases: local repository creation, GitHub repository creation and repository linking.

### 1.1 Local Repository Creation

First, let's create a Git repository on your local machine:

1. Open your terminal.
2. Navigate to the directory where you want to create the repository.
   ```bash
   cd path/to/your/directory
   ```

3. Run the following command to initialize a new Git repository:
   ```bash
   git init First-Git-Exercise
   ```

### 1.2 GitHub Repository Creation

Now, let's create a corresponding repository on GitHub:

1. Go to [GitHub](https://github.com/) and log in to your account.

2. Click on the "+" sign in the top right corner and select "New repository."

3. Name your repository "First-Git-Exercise", add a brief description and set it **Private**

4. Click "Create repository."

### 1.3 Repository Linking

We have created two distinct repos. But how do we link them? We have to create a reference between the local repository and the **remote** one.

Link your local repository to the GitHub repository as follows:
   ```bash
   git remote add origin https://github.com/your-username/First-Git-Exercise.git
   ```

   Replace `your-username` with your *actual* GitHub username.

