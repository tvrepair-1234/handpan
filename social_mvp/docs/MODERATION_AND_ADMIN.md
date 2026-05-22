# Moderation and Admin Logic

## User Moderation
- Report post/user with reason.
- Block user prevents direct interaction and hides content in feed.

## Admin Moderation
- Review open reports queue.
- Remove post (soft remove via `is_removed=true`).
- Ban user (`profiles.is_banned=true`).
- Record action in `admin_actions`.

## Operational guidance
- Keep admin APIs server-side only (Edge Function/service role).
- Log all high-risk actions.
- Add abuse thresholds later (repeat offenders, spam heuristics).
