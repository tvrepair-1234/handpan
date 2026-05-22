# Implementation Plan

## 1. Discovery
- Confirm product scope is community-first MVP.
- Exclude advanced video/recommendation/monetization for V1.

## 2. Data layer first (Supabase)
- Create core tables (`profiles`, `communities`, `posts`, `comments`, `follows`, `likes`, etc.).
- Add RLS policies for all user-facing operations.
- Add indexes for feed and notifications.

## 3. Flutter skeleton
- Module per feature.
- Shared core for routing/session/config.
- Keep backend access behind repositories.

## 4. MVP functional steps
1. Auth + profile bootstrap.
2. Create/join communities.
3. Create/list posts (text/image).
4. Like/comment/delete post.
5. Home/community feeds.
6. Follow/search/bookmark.
7. Notifications.
8. Reports/blocks/admin actions.

## 5. Stabilization
- Add unit tests for feed filters and access rules.
- Add integration tests for auth and posting flow.
- Add moderation scenario tests.

## 6. Launch prep
- Configure environments and secrets.
- Add monitoring + analytics.
- Prepare staged rollout.
