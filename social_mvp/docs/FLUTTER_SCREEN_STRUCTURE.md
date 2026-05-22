# Flutter Screen Structure

## App Shell
- Splash/SessionRestoreScreen
- AuthGate
- MainTabScaffold
  - HomeFeedScreen
  - CommunitiesScreen
  - NotificationsScreen
  - ProfileScreen

## Auth
- LoginScreen
- SignUpScreen
- ForgotPasswordScreen
- ResetPasswordScreen

## Profile
- EditProfileScreen
- UserProfileScreen (with follow button)
- FollowersListScreen / FollowingListScreen

## Communities
- CommunityListScreen
- CommunityDetailScreen
- CommunityCreateScreen
- CommunityMembersScreen

## Posts
- CreatePostScreen
- PostDetailScreen
- CommentsSheet
- BookmarksScreen

## Social/Search
- SearchScreen (users + communities)

## Moderation/Admin
- ReportSheet
- BlockedUsersScreen
- AdminQueueScreen
- AdminUserActionsScreen

## Suggested folders
- `features/<feature>/data`
- `features/<feature>/domain`
- `features/<feature>/presentation`
