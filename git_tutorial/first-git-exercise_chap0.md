# First Git Exercise

## Introduction

This is your first exercise with Git. Some of you have already worked with this technology, and will take this assignment lightly. *That's not a good idea*. 

## Before we start

Before we start, we need to set up some things.

### GitHub Account Creation

Our first requirement is a GitHub account. There are many other possibilities, eg. Bitbucket, GitLab, etc. We chose GitHub for both its simplicity and its popularity. 

### (Recommended) SSH Keys setup

It is recommended to setup an SSH key following these steps:

1. [key creation](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent)

2. [SSH key addition to GitHub account](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/adding-a-new-ssh-key-to-your-github-account)

### git configuration

We need to take a couple more steps in order to make git work properly. In fact, without a username and an email, the actions we take cannot be accounted.

```bash
git config --global user.name "Name Surname" # This name will be associated with every action you take.

git config --global user.email email@example.com # GitHub uses this email address to associate commits with your account on GitHub. Hence, the addresses should match.

git config --global --list # Check the configuration. Now you should see the values that you prompted.
```

Using git we can find two different types of configurations: *local* and *global*. The local configuration, if not edited, inherits the values from the global one. In addition to those values, some more, related to the specific project, are added.

We will see more on the local configuration in the next chapter.