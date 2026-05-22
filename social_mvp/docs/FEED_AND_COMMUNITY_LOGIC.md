# Feed and Community Logic

## Home Feed Query (chronological)
Include posts where:
- `author_id` is in followed users, OR
- `community_id` is in joined communities.

Exclude:
- removed posts.
- blocked users (both directions where applicable).

Order:
- `created_at DESC`.

## Community Feed
- Filter by `community_id`.
- Require membership for private communities (if enabled later).

## Post creation rules
- Community post requires membership.
- User can post on personal timeline (null `community_id`) if product supports it.

## Future-ready for video
- Keep feed service interface generic:
  - `fetchFeed({cursor, filters, mediaType})`
- Later add ranking strategy object without breaking API consumers.
