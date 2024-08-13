# QoolTest
This is a test management tool which allows user to create, modify and save test cases.

User has also the option to mark test cases with results, such as "Neutral", "Pass" and "Fail".

All test results can be cleaned up via `Edit`|`Clear Results`.

## Editing Test Cases
Select one test case, then use the utilities at the buttom right corner. Test steps can be added, deleted or moved around. Do not forget to click `Save`.

## Managing Test Cases
- `Follow`: click on an existing test case under tree view and input the UID to follow; choose `as Child` if a deeper layer is desired. As result, the test case is moved behind the UID.
- `Add Subfolder`: input UID, a new test folder with a new UID will be added behind the UID.
- `Add Testcase`: same as `Add Subfolder`, but as test case.
- `Delete`: test case/folder of selected UID including its subcases are going to be removed.

## Tips
* Click `Save` after test case has been modified.
* The modifications on test cases only change the project data in the memory, do not forget to save project as qlpj-file frequently.

## Preview
![image info](assets/screenshots/screen_cases.PNG)