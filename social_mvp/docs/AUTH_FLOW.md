# Auth Flow

1. User opens app.
2. `SessionManager` checks active Supabase session.
3. If no session -> Auth screens.
4. Sign-up:
   - Email/password create user.
   - Insert profile row.
5. Login:
   - Email/password sign-in.
6. Forgot password:
   - Send reset email via Supabase Auth.
   - Handle deep link to reset screen.
7. Session handling:
   - Refresh token managed by Supabase SDK.
   - On logout, clear local cache and return to auth gate.
