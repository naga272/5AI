# First Git Exercise

## Chapter 2: Our First README

For creating our first file, we proceed with two distinct actions: the creation of the file on the local repository and the update of the remote repository.

### 2.1 Create a README

We are now ready for creating our first file!
Let's create a README file for your project:

1. In your local repository directory, create a new file named `README.md`.

2. Open the `README.md` file in a text editor.

3. Add some content to your README. For example:
   ```markdown
   # First Git Exercise

   This is the README file for our first Git exercise repository.
   ```

We have now created the file. What happens if we execute the following command?

   ```bash
   git status
   ```

We can see that the file we just created is recognised as *untracked*. In which **Git area** is the file?

### 2.2 Commit and Push

Now, let's commit and push your changes to both your local and remote repositories. We need these actions in order to save the work that has been done.

1. In your terminal, navigate to the repository directory:
   ```bash
   cd path/to/First-Git-Exercise
   ```

2. We now need to move our modifications to the next area, closer to the remote repository. Hence, we stage the changes:

   ```bash
   git add README.md
   ```

   Again, what happens if we run `git status`?

3. Commit the changes:
   ```bash
   git commit -m "Add README file"
   ```

   Once again, what happens if we run `git status`?

   We can also run the following command to see what have exactly changed.

   ```bash
   git diff master origin/master
   ```

5. Push the changes to GitHub:
   ```bash
   git push -u origin master
   ```

   One last time, run `git status`.

Now, your local and remote repositories are in sync, and you have successfully created a Git repository with an initial README file.

Before ending, let's reason about a thing: what happens someone else is adding a new file and we modify the master branch?