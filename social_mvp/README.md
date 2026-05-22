# Community Social App MVP (Flutter + Supabase)

This folder contains a production-oriented **MVP blueprint** for an interest/community-based social app that can later evolve into a short-video product.

## Goals
- Ship a secure, simple V1 quickly.
- Prioritize retention and healthy communities.
- Keep architecture extensible for future vertical video features.

## Tech stack
- Frontend: Flutter
- Backend/Auth/DB/Storage: Supabase
- Database: PostgreSQL
- Push: Firebase Cloud Messaging (FCM)
- Analytics: PostHog (or Firebase Analytics)

## Folder structure

```text
social_mvp/
  BUSINESS_PLAN.md
  README.md
  docs/
    IMPLEMENTATION_PLAN.md
    FLUTTER_SCREEN_STRUCTURE.md
    AUTH_FLOW.md
    FEED_AND_COMMUNITY_LOGIC.md
    MODERATION_AND_ADMIN.md
    TESTING_CHECKLIST.md
    DEPLOYMENT_GUIDE.md
  supabase/
    schema.sql
  flutter_app/
    lib/
      main.dart
      app.dart
      core/
        router.dart
        config.dart
        auth_session_manager.dart
      features/
        auth/
        profile/
        communities/
        posts/
        feed/
        social/
        notifications/
        moderation/
        admin/
    test/
      smoke_test_plan.md
```

## What is implemented in this MVP scaffold
1. **Supabase schema and RLS** for users, communities, posts, comments, followers, likes, bookmarks, notifications, reports, blocks, and admin actions.
2. **Flutter app skeleton** and screen/module boundaries for clean growth.
3. **Auth/session flow** (signup/login/reset/session restore).
4. **Feed/community/moderation logic design**.
5. **Testing and deployment guides**.
6. **Business plan and phased monetization roadmap**.

## MVP Feature Mapping

### 1) Authentication
- Email/password sign-up + login.
- Password reset.
- Session restore + guarded routes.

### 2) Profiles
- Username, bio, avatar URL.
- Follower/following counts through materialized views or query aggregation.

### 3) Communities
- Create/join community.
- Name/description/image + member table.
- Community feed by `community_id`.

### 4) Posts
- Text and optional image.
- Like/unlike, comments.
- User can delete own posts.

### 5) Feed
- Home feed = joined communities + followed users.
- Community feed by community.
- Chronological by `created_at DESC`.

### 6) Social
- Follow/unfollow.
- Bookmark/unbookmark posts.
- Basic search for users + communities.

### 7) Notifications
- New follower.
- New like.
- New comment.

### 8) Moderation
- Report post/user.
- Block user.
- Admin remove posts and ban users.

## Video-ready architecture (without implementing video now)
The schema already includes extension points:
- `posts.content_type` enum with `text`, `image`, `video`.
- `media_assets` table supports future video metadata (duration/height/width/processing state).
- Feed builder isolates ranking strategy so chronological can later evolve into hybrid ranking.

## Next steps to run
1. Create a new Supabase project.
2. Run `supabase/schema.sql` in SQL editor.
3. Configure env vars in Flutter (`SUPABASE_URL`, `SUPABASE_ANON_KEY`).
4. Scaffold Flutter screens from docs and wire Supabase queries.
5. Set up FCM and analytics.

See docs in `social_mvp/docs/` for implementation details.
