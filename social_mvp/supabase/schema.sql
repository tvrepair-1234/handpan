-- Community Social App MVP Schema (Supabase/PostgreSQL)
create extension if not exists pgcrypto;

create type post_content_type as enum ('text', 'image', 'video');
create type notification_type as enum ('new_follower', 'post_like', 'post_comment');
create type report_target_type as enum ('post', 'user');

create table if not exists public.profiles (
  id uuid primary key references auth.users(id) on delete cascade,
  username text unique not null check (char_length(username) between 3 and 30),
  bio text default '',
  avatar_url text,
  is_banned boolean not null default false,
  created_at timestamptz not null default now(),
  updated_at timestamptz not null default now()
);

create table if not exists public.user_blocks (
  blocker_id uuid not null references public.profiles(id) on delete cascade,
  blocked_id uuid not null references public.profiles(id) on delete cascade,
  created_at timestamptz not null default now(),
  primary key (blocker_id, blocked_id),
  check (blocker_id <> blocked_id)
);

create table if not exists public.follows (
  follower_id uuid not null references public.profiles(id) on delete cascade,
  following_id uuid not null references public.profiles(id) on delete cascade,
  created_at timestamptz not null default now(),
  primary key (follower_id, following_id),
  check (follower_id <> following_id)
);

create table if not exists public.communities (
  id uuid primary key default gen_random_uuid(),
  name text unique not null,
  description text not null default '',
  image_url text,
  created_by uuid not null references public.profiles(id) on delete restrict,
  created_at timestamptz not null default now(),
  updated_at timestamptz not null default now()
);

create table if not exists public.community_members (
  community_id uuid not null references public.communities(id) on delete cascade,
  user_id uuid not null references public.profiles(id) on delete cascade,
  role text not null default 'member' check (role in ('member', 'moderator', 'owner')),
  created_at timestamptz not null default now(),
  primary key (community_id, user_id)
);

create table if not exists public.posts (
  id uuid primary key default gen_random_uuid(),
  author_id uuid not null references public.profiles(id) on delete cascade,
  community_id uuid references public.communities(id) on delete set null,
  content_type post_content_type not null default 'text',
  text_content text,
  image_url text,
  is_removed boolean not null default false,
  removed_reason text,
  created_at timestamptz not null default now(),
  updated_at timestamptz not null default now(),
  check (text_content is not null or image_url is not null)
);

create table if not exists public.comments (
  id uuid primary key default gen_random_uuid(),
  post_id uuid not null references public.posts(id) on delete cascade,
  author_id uuid not null references public.profiles(id) on delete cascade,
  body text not null,
  created_at timestamptz not null default now()
);

create table if not exists public.post_likes (
  post_id uuid not null references public.posts(id) on delete cascade,
  user_id uuid not null references public.profiles(id) on delete cascade,
  created_at timestamptz not null default now(),
  primary key (post_id, user_id)
);

create table if not exists public.bookmarks (
  post_id uuid not null references public.posts(id) on delete cascade,
  user_id uuid not null references public.profiles(id) on delete cascade,
  created_at timestamptz not null default now(),
  primary key (post_id, user_id)
);

create table if not exists public.notifications (
  id uuid primary key default gen_random_uuid(),
  actor_id uuid references public.profiles(id) on delete set null,
  recipient_id uuid not null references public.profiles(id) on delete cascade,
  type notification_type not null,
  post_id uuid references public.posts(id) on delete cascade,
  comment_id uuid references public.comments(id) on delete cascade,
  is_read boolean not null default false,
  created_at timestamptz not null default now()
);

create table if not exists public.reports (
  id uuid primary key default gen_random_uuid(),
  reporter_id uuid not null references public.profiles(id) on delete cascade,
  target_type report_target_type not null,
  target_post_id uuid references public.posts(id) on delete cascade,
  target_user_id uuid references public.profiles(id) on delete cascade,
  reason text not null,
  status text not null default 'open' check (status in ('open', 'reviewing', 'resolved', 'dismissed')),
  created_at timestamptz not null default now(),
  check (
    (target_type = 'post' and target_post_id is not null and target_user_id is null)
    or
    (target_type = 'user' and target_user_id is not null and target_post_id is null)
  )
);

create table if not exists public.admin_actions (
  id uuid primary key default gen_random_uuid(),
  admin_id uuid not null references public.profiles(id) on delete restrict,
  action_type text not null check (action_type in ('remove_post', 'ban_user', 'unban_user')),
  target_post_id uuid references public.posts(id) on delete set null,
  target_user_id uuid references public.profiles(id) on delete set null,
  notes text,
  created_at timestamptz not null default now()
);

create table if not exists public.media_assets (
  id uuid primary key default gen_random_uuid(),
  post_id uuid not null references public.posts(id) on delete cascade,
  media_type post_content_type not null,
  storage_path text not null,
  width int,
  height int,
  duration_seconds int,
  processing_status text not null default 'ready' check (processing_status in ('queued', 'processing', 'ready', 'failed')),
  created_at timestamptz not null default now()
);

create index if not exists idx_posts_created_at on public.posts (created_at desc);
create index if not exists idx_posts_community_created on public.posts (community_id, created_at desc);
create index if not exists idx_follows_following on public.follows (following_id);
create index if not exists idx_notifications_recipient_created on public.notifications (recipient_id, created_at desc);

alter table public.profiles enable row level security;
alter table public.user_blocks enable row level security;
alter table public.follows enable row level security;
alter table public.communities enable row level security;
alter table public.community_members enable row level security;
alter table public.posts enable row level security;
alter table public.comments enable row level security;
alter table public.post_likes enable row level security;
alter table public.bookmarks enable row level security;
alter table public.notifications enable row level security;
alter table public.reports enable row level security;
alter table public.admin_actions enable row level security;
alter table public.media_assets enable row level security;

create policy "profiles_select_public" on public.profiles for select using (true);
create policy "profiles_update_self" on public.profiles for update using (auth.uid() = id);
create policy "profiles_insert_self" on public.profiles for insert with check (auth.uid() = id);

create policy "follows_read_all" on public.follows for select using (true);
create policy "follows_insert_self" on public.follows for insert with check (auth.uid() = follower_id);
create policy "follows_delete_self" on public.follows for delete using (auth.uid() = follower_id);

create policy "communities_read_all" on public.communities for select using (true);
create policy "communities_create_auth" on public.communities for insert with check (auth.uid() = created_by);
create policy "communities_update_owner" on public.communities for update using (
  exists (
    select 1 from public.community_members cm
    where cm.community_id = communities.id and cm.user_id = auth.uid() and cm.role in ('owner', 'moderator')
  )
);

create policy "community_members_read_all" on public.community_members for select using (true);
create policy "community_members_join_self" on public.community_members for insert with check (auth.uid() = user_id);
create policy "community_members_leave_self" on public.community_members for delete using (auth.uid() = user_id);

create policy "posts_read_visible" on public.posts for select using (is_removed = false);
create policy "posts_insert_member_or_profile" on public.posts for insert with check (
  auth.uid() = author_id and (
    community_id is null or exists (
      select 1 from public.community_members cm
      where cm.community_id = posts.community_id and cm.user_id = auth.uid()
    )
  )
);
create policy "posts_delete_own" on public.posts for delete using (auth.uid() = author_id);

create policy "comments_read_all" on public.comments for select using (true);
create policy "comments_insert_self" on public.comments for insert with check (auth.uid() = author_id);
create policy "comments_delete_own" on public.comments for delete using (auth.uid() = author_id);

create policy "likes_read_all" on public.post_likes for select using (true);
create policy "likes_insert_self" on public.post_likes for insert with check (auth.uid() = user_id);
create policy "likes_delete_self" on public.post_likes for delete using (auth.uid() = user_id);

create policy "bookmarks_read_own" on public.bookmarks for select using (auth.uid() = user_id);
create policy "bookmarks_insert_own" on public.bookmarks for insert with check (auth.uid() = user_id);
create policy "bookmarks_delete_own" on public.bookmarks for delete using (auth.uid() = user_id);

create policy "notifications_read_own" on public.notifications for select using (auth.uid() = recipient_id);
create policy "notifications_update_own" on public.notifications for update using (auth.uid() = recipient_id);

create policy "blocks_read_own" on public.user_blocks for select using (auth.uid() = blocker_id);
create policy "blocks_insert_own" on public.user_blocks for insert with check (auth.uid() = blocker_id);
create policy "blocks_delete_own" on public.user_blocks for delete using (auth.uid() = blocker_id);

create policy "reports_insert_self" on public.reports for insert with check (auth.uid() = reporter_id);
create policy "reports_read_self_or_admin" on public.reports for select using (
  auth.uid() = reporter_id or auth.jwt() ->> 'role' = 'service_role'
);

create policy "admin_actions_admin_only" on public.admin_actions for all using (
  auth.jwt() ->> 'role' = 'service_role'
) with check (
  auth.jwt() ->> 'role' = 'service_role'
);

create policy "media_assets_read_all" on public.media_assets for select using (true);
create policy "media_assets_insert_post_owner" on public.media_assets for insert with check (
  exists (select 1 from public.posts p where p.id = media_assets.post_id and p.author_id = auth.uid())
);
