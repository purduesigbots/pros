# Contributing to PROS

:tada: :+1: :steam_locomotive: Thanks for taking the time to contribute! :steam_locomotive: :+1: :tada:

**Did you find a bug?**
- **Before opening an issue, make sure you are in the right repository!**
  purduesigbots maintains four repositories related to PROS:
  - [purduesigbots/pros](https://github.com/purduesigbots/pros): the repository containing the source code for the kernel the user-facing API. Issues should be opened here if they affect the code you write (e.g., "I would like to be able to do X with PROS," or "when I call <PROS function> X doesn't work as I expect")
  - [purduesigbots/pros-cli](https://github.com/purduesigbots/pros-cli): the repository containing the source code for the command line interface (CLI). Issues should be opened here if they concern the PROS CLI (e.g., problems with commands like `pros make`), as well as project creation and management.
  - [purduesigbots/pros-atom](https://github.com/purduesigbots/pros-atom): the repository containing the source code for the Atom package. Issues should be opened here if they concern the coding experience within Atom (e.g., "there is no button to do X," or "the linter is spamming my interface with errors").
  - [purduesigbots/pros-docs](https://github.com/purduesigbots/pros-docs): the repository containing the source code for [our documentation website](https://pros.cs.purdue.edu). Issues should be opened here if they concern available documentation (e.g., "there is not guide on using <PROS feature>," or "the documentation says to do X, but only Y works")
- **Verify the bug lies in PROS.** We receive quite a few reports that are due to bugs in user code, not the kernel.
- Ensure the bug wasn't already reported by searching GitHub [issues](https://github.com/purduesigbots/pros/issues)
- If you're unable to find an issue, [open](https://github.com/purduesigbots/pros/issues/new) a new one.

**Did you patch a bug or add a new feature?**
1. [Fork](https://github.com/purduesigbots/pros/fork) and clone the repository
2. Create a new branch: `git checkout -b my-branch-name`
3. Make your changes.
4. Push to your fork and submit a pull request.
5. Wait for your pull request to be reviewed. In order to ensure that the PROS kernel is stable, we take extra time to test pull requests. As a result, your pull request may take some time to be merged into master.

Here are a few tips that can help expedite your pull request being accepted:
- Follow existing code's style.
- Document why you made the changes you did.
- Keep your change as focused as possible. If you have multiple independent changes, make a pull request for each.
- If you did some testing, describe your procedure and results.
- If you're fixing an issue, reference it by number.
