# Deployment Guide

## Environments
- Dev: local Flutter + Supabase project.
- Staging: production-like Supabase + test app distribution.
- Prod: locked secrets, monitoring, backups.

## Supabase setup
1. Create project.
2. Execute `supabase/schema.sql`.
3. Configure Auth providers (email/password).
4. Configure Storage buckets (profile, posts).
5. Add Edge Functions for admin-only operations.

## Flutter setup
1. Add env config for Supabase URL + anon key.
2. Configure Firebase for push notifications.
3. Configure analytics SDK.
4. Build and distribute:
   - Android: Play internal test track.
   - iOS: TestFlight.

## Production safeguards
- Enable database backups.
- Enable rate limits (auth and write endpoints).
- Enable alerting (errors, failed logins, API latency).
- Add content moderation playbook and on-call owner.
