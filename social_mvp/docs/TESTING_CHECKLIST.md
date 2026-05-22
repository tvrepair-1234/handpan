# Testing Checklist

## Auth
- [ ] Sign-up success/failure cases.
- [ ] Login success/failure cases.
- [ ] Reset password flow.
- [ ] Session restore after app restart.

## Profiles
- [ ] Update username/bio/avatar.
- [ ] Follower/following counters accurate.

## Communities
- [ ] Create community.
- [ ] Join/leave community.
- [ ] Community feed shows correct posts.

## Posts
- [ ] Create text post.
- [ ] Create image post.
- [ ] Like/unlike idempotency.
- [ ] Add comments.
- [ ] Delete own post only.

## Feed/Social
- [ ] Home feed includes followed users + joined communities.
- [ ] Chronological ordering.
- [ ] Bookmark add/remove and retrieval.
- [ ] Search users and communities.

## Notifications
- [ ] Follow notification created.
- [ ] Like notification created.
- [ ] Comment notification created.

## Moderation
- [ ] Report submission.
- [ ] Block user hides content.
- [ ] Admin remove post.
- [ ] Admin ban/unban user.

## Security
- [ ] RLS prevents unauthorized reads/writes.
- [ ] Banned users cannot post/interact (policy or service checks).
