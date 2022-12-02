/*
 * This contains encryption functions for per-file encryption.
 *
 * Copyright (C) 2015, Google, Inc.
 * Copyright (C) 2015, Motorola Mobility
 *
 * Written by Michael Halcrow, 2014.
 *
 * Filename encryption additions
 *	Uday Savagaonkar, 2014
 * Encryption policy handling additions
 *	Ildar Muslukhov, 2014
 * Add fscrypt_pullback_bio_page()
 *	Jaegeuk Kim, 2015.
 *
 * This has not yet undergone a rigorous security audit.
 *
 * The usage of AES-XTS should conform to recommendations in NIST
 * Special Publication 800-38E and IEEE P1619/D16.
 */

#include <linux/pagemap.h>
#include <linux/mempool.h>
#include <linux/module.h>
#include <linux/scatterlist.h>
#include <linux/ratelimit.h>
<<<<<<< HEAD
#include <crypto/skcipher.h>
#include "fscrypt_private.h"
#ifdef CONFIG_FSCRYPT_SDP
#include "sdp/sdp_crypto.h"
#endif

static unsigned int num_prealloc_crypto_pages = 32;
=======
#include <linux/dcache.h>
#include <linux/namei.h>
#include <crypto/aes.h>
#include <crypto/skcipher.h>
#include "fscrypt_private.h"

static unsigned int num_prealloc_crypto_pages = 32;
static unsigned int num_prealloc_crypto_ctxs = 128;
>>>>>>> rebase

module_param(num_prealloc_crypto_pages, uint, 0444);
MODULE_PARM_DESC(num_prealloc_crypto_pages,
		"Number of crypto pages to preallocate");
<<<<<<< HEAD

static mempool_t *fscrypt_bounce_page_pool = NULL;

static struct workqueue_struct *fscrypt_read_workqueue;
static DEFINE_MUTEX(fscrypt_init_mutex);

=======
module_param(num_prealloc_crypto_ctxs, uint, 0444);
MODULE_PARM_DESC(num_prealloc_crypto_ctxs,
		"Number of crypto contexts to preallocate");

static mempool_t *fscrypt_bounce_page_pool = NULL;

static LIST_HEAD(fscrypt_free_ctxs);
static DEFINE_SPINLOCK(fscrypt_ctx_lock);

static struct workqueue_struct *fscrypt_read_workqueue;
static DEFINE_MUTEX(fscrypt_init_mutex);

static struct kmem_cache *fscrypt_ctx_cachep;
>>>>>>> rebase
struct kmem_cache *fscrypt_info_cachep;

void fscrypt_enqueue_decrypt_work(struct work_struct *work)
{
	queue_work(fscrypt_read_workqueue, work);
}
EXPORT_SYMBOL(fscrypt_enqueue_decrypt_work);

<<<<<<< HEAD
struct page *fscrypt_alloc_bounce_page(gfp_t gfp_flags)
{
	return mempool_alloc(fscrypt_bounce_page_pool, gfp_flags);
}

/**
 * fscrypt_free_bounce_page() - free a ciphertext bounce page
 *
 * Free a bounce page that was allocated by fscrypt_encrypt_pagecache_blocks(),
 * or by fscrypt_alloc_bounce_page() directly.
 */
void fscrypt_free_bounce_page(struct page *bounce_page)
{
	if (!bounce_page)
		return;
	set_page_private(bounce_page, (unsigned long)NULL);
	ClearPagePrivate(bounce_page);
	mempool_free(bounce_page, fscrypt_bounce_page_pool);
}
EXPORT_SYMBOL(fscrypt_free_bounce_page);

/*
 * Generate the IV for the given logical block number within the given file.
 * For filenames encryption, lblk_num == 0.
 *
 * Keep this in sync with fscrypt_limit_dio_pages().  fscrypt_limit_dio_pages()
 * needs to know about any IV generation methods where the low bits of IV don't
 * simply contain the lblk_num (e.g., IV_INO_LBLK_32).
 */
void fscrypt_generate_iv(union fscrypt_iv *iv, u64 lblk_num,
			 const struct fscrypt_info *ci)
{
	u8 flags = fscrypt_policy_flags(&ci->ci_policy);

	bool inlinecrypt = false;

#ifdef CONFIG_FS_ENCRYPTION_INLINE_CRYPT
	inlinecrypt = ci->ci_inlinecrypt;
#endif
	memset(iv, 0, ci->ci_mode->ivsize);

	if (flags & FSCRYPT_POLICY_FLAG_IV_INO_LBLK_64 ||
		((fscrypt_policy_contents_mode(&ci->ci_policy) ==
		  FSCRYPT_MODE_PRIVATE) && inlinecrypt)) {
		WARN_ON_ONCE(lblk_num > U32_MAX);
		WARN_ON_ONCE(ci->ci_inode->i_ino > U32_MAX);
		lblk_num |= (u64)ci->ci_inode->i_ino << 32;
	} else if (flags & FSCRYPT_POLICY_FLAG_IV_INO_LBLK_32) {
		WARN_ON_ONCE(lblk_num > U32_MAX);
		lblk_num = (u32)(ci->ci_hashed_ino + lblk_num);
	} else if (flags & FSCRYPT_POLICY_FLAG_DIRECT_KEY) {
		memcpy(iv->nonce, ci->ci_nonce, FS_KEY_DERIVATION_NONCE_SIZE);
	}
	iv->lblk_num = cpu_to_le64(lblk_num);
}

/* Encrypt or decrypt a single filesystem block of file contents */
int fscrypt_crypt_block(const struct inode *inode, fscrypt_direction_t rw,
			u64 lblk_num, struct page *src_page,
			struct page *dest_page, unsigned int len,
			unsigned int offs, gfp_t gfp_flags)
{
	union fscrypt_iv iv;
=======
/**
 * fscrypt_release_ctx() - Releases an encryption context
 * @ctx: The encryption context to release.
 *
 * If the encryption context was allocated from the pre-allocated pool, returns
 * it to that pool. Else, frees it.
 *
 * If there's a bounce page in the context, this frees that.
 */
void fscrypt_release_ctx(struct fscrypt_ctx *ctx)
{
	unsigned long flags;

	if (ctx->flags & FS_CTX_HAS_BOUNCE_BUFFER_FL && ctx->w.bounce_page) {
		mempool_free(ctx->w.bounce_page, fscrypt_bounce_page_pool);
		ctx->w.bounce_page = NULL;
	}
	ctx->w.control_page = NULL;
	if (ctx->flags & FS_CTX_REQUIRES_FREE_ENCRYPT_FL) {
		kmem_cache_free(fscrypt_ctx_cachep, ctx);
	} else {
		spin_lock_irqsave(&fscrypt_ctx_lock, flags);
		list_add(&ctx->free_list, &fscrypt_free_ctxs);
		spin_unlock_irqrestore(&fscrypt_ctx_lock, flags);
	}
}
EXPORT_SYMBOL(fscrypt_release_ctx);

/**
 * fscrypt_get_ctx() - Gets an encryption context
 * @inode:       The inode for which we are doing the crypto
 * @gfp_flags:   The gfp flag for memory allocation
 *
 * Allocates and initializes an encryption context.
 *
 * Return: An allocated and initialized encryption context on success; error
 * value or NULL otherwise.
 */
struct fscrypt_ctx *fscrypt_get_ctx(const struct inode *inode, gfp_t gfp_flags)
{
	struct fscrypt_ctx *ctx = NULL;
	struct fscrypt_info *ci = inode->i_crypt_info;
	unsigned long flags;

	if (ci == NULL)
		return ERR_PTR(-ENOKEY);

	/*
	 * We first try getting the ctx from a free list because in
	 * the common case the ctx will have an allocated and
	 * initialized crypto tfm, so it's probably a worthwhile
	 * optimization. For the bounce page, we first try getting it
	 * from the kernel allocator because that's just about as fast
	 * as getting it from a list and because a cache of free pages
	 * should generally be a "last resort" option for a filesystem
	 * to be able to do its job.
	 */
	spin_lock_irqsave(&fscrypt_ctx_lock, flags);
	ctx = list_first_entry_or_null(&fscrypt_free_ctxs,
					struct fscrypt_ctx, free_list);
	if (ctx)
		list_del(&ctx->free_list);
	spin_unlock_irqrestore(&fscrypt_ctx_lock, flags);
	if (!ctx) {
		ctx = kmem_cache_zalloc(fscrypt_ctx_cachep, gfp_flags);
		if (!ctx)
			return ERR_PTR(-ENOMEM);
		ctx->flags |= FS_CTX_REQUIRES_FREE_ENCRYPT_FL;
	} else {
		ctx->flags &= ~FS_CTX_REQUIRES_FREE_ENCRYPT_FL;
	}
	ctx->flags &= ~FS_CTX_HAS_BOUNCE_BUFFER_FL;
	return ctx;
}
EXPORT_SYMBOL(fscrypt_get_ctx);

int fscrypt_do_page_crypto(const struct inode *inode, fscrypt_direction_t rw,
			   u64 lblk_num, struct page *src_page,
			   struct page *dest_page, unsigned int len,
			   unsigned int offs, gfp_t gfp_flags)
{
	struct {
		__le64 index;
		u8 padding[FS_IV_SIZE - sizeof(__le64)];
	} iv;
>>>>>>> rebase
	struct skcipher_request *req = NULL;
	DECLARE_CRYPTO_WAIT(wait);
	struct scatterlist dst, src;
	struct fscrypt_info *ci = inode->i_crypt_info;
<<<<<<< HEAD
	struct crypto_skcipher *tfm = ci->ci_key.tfm;
	int res = 0;
#ifdef CONFIG_FSCRYPT_SDP
	sdp_fs_command_t *cmd = NULL;
#endif
=======
	struct crypto_skcipher *tfm = ci->ci_ctfm;
	int res = 0;
>>>>>>> rebase

	if (WARN_ON_ONCE(len <= 0))
		return -EINVAL;
	if (WARN_ON_ONCE(len % FS_CRYPTO_BLOCK_SIZE != 0))
		return -EINVAL;

<<<<<<< HEAD
	fscrypt_generate_iv(&iv, lblk_num, ci);
=======
	BUILD_BUG_ON(sizeof(iv) != FS_IV_SIZE);
	BUILD_BUG_ON(AES_BLOCK_SIZE != FS_IV_SIZE);
	iv.index = cpu_to_le64(lblk_num);
	memset(iv.padding, 0, sizeof(iv.padding));

	if (ci->ci_essiv_tfm != NULL) {
		crypto_cipher_encrypt_one(ci->ci_essiv_tfm, (u8 *)&iv,
					  (u8 *)&iv);
	}
>>>>>>> rebase

	req = skcipher_request_alloc(tfm, gfp_flags);
	if (!req)
		return -ENOMEM;

	skcipher_request_set_callback(
		req, CRYPTO_TFM_REQ_MAY_BACKLOG | CRYPTO_TFM_REQ_MAY_SLEEP,
		crypto_req_done, &wait);

	sg_init_table(&dst, 1);
	sg_set_page(&dst, dest_page, len, offs);
	sg_init_table(&src, 1);
	sg_set_page(&src, src_page, len, offs);
	skcipher_request_set_crypt(req, &src, &dst, len, &iv);
	if (rw == FS_DECRYPT)
		res = crypto_wait_req(crypto_skcipher_decrypt(req), &wait);
	else
		res = crypto_wait_req(crypto_skcipher_encrypt(req), &wait);
	skcipher_request_free(req);
	if (res) {
<<<<<<< HEAD
		fscrypt_err(inode, "%scryption failed for block %llu: %d",
			    (rw == FS_DECRYPT ? "De" : "En"), lblk_num, res);
#ifdef CONFIG_FSCRYPT_SDP
		if (ci->ci_sdp_info) {
			if (ci->ci_sdp_info->sdp_flags & SDP_DEK_IS_SENSITIVE) {
				printk("Record audit log in case of a failure during en/decryption of sensitive file\n");
				if (rw == FS_DECRYPT) {
					cmd = sdp_fs_command_alloc(FSOP_AUDIT_FAIL_DECRYPT,
					current->tgid, ci->ci_sdp_info->engine_id, -1, inode->i_ino, res,
							GFP_KERNEL);
				} else {
					cmd = sdp_fs_command_alloc(FSOP_AUDIT_FAIL_ENCRYPT,
					current->tgid, ci->ci_sdp_info->engine_id, -1, inode->i_ino, res,
							GFP_KERNEL);
				}
				if (cmd) {
					sdp_fs_request(cmd, NULL);
					sdp_fs_command_free(cmd);
				}
			}
		}
#endif
=======
		fscrypt_err(inode->i_sb,
			    "%scryption failed for inode %lu, block %llu: %d",
			    (rw == FS_DECRYPT ? "de" : "en"),
			    inode->i_ino, lblk_num, res);
>>>>>>> rebase
		return res;
	}
	return 0;
}

<<<<<<< HEAD
/**
 * fscrypt_encrypt_pagecache_blocks() - Encrypt filesystem blocks from a pagecache page
 * @page:      The locked pagecache page containing the block(s) to encrypt
 * @len:       Total size of the block(s) to encrypt.  Must be a nonzero
 *		multiple of the filesystem's block size.
 * @offs:      Byte offset within @page of the first block to encrypt.  Must be
 *		a multiple of the filesystem's block size.
 * @gfp_flags: Memory allocation flags.  See details below.
 *
 * A new bounce page is allocated, and the specified block(s) are encrypted into
 * it.  In the bounce page, the ciphertext block(s) will be located at the same
 * offsets at which the plaintext block(s) were located in the source page; any
 * other parts of the bounce page will be left uninitialized.  However, normally
 * blocksize == PAGE_SIZE and the whole page is encrypted at once.
 *
 * This is for use by the filesystem's ->writepages() method.
 *
 * The bounce page allocation is mempool-backed, so it will always succeed when
 * @gfp_flags includes __GFP_DIRECT_RECLAIM, e.g. when it's GFP_NOFS.  However,
 * only the first page of each bio can be allocated this way.  To prevent
 * deadlocks, for any additional pages a mask like GFP_NOWAIT must be used.
 *
 * Return: the new encrypted bounce page on success; an ERR_PTR() on failure
 */
struct page *fscrypt_encrypt_pagecache_blocks(struct page *page,
					      unsigned int len,
					      unsigned int offs,
					      gfp_t gfp_flags)

{
	const struct inode *inode = page->mapping->host;
	const unsigned int blockbits = inode->i_blkbits;
	const unsigned int blocksize = 1 << blockbits;
	struct page *ciphertext_page;
	u64 lblk_num = ((u64)page->index << (PAGE_SHIFT - blockbits)) +
		       (offs >> blockbits);
	unsigned int i;
	int err;

#ifdef CONFIG_DDAR
	if (fscrypt_dd_encrypted_inode(inode)) {
		// Invert crypto order. OEM crypto must perform after 3rd party crypto
		return NULL;
	}
#endif
=======
struct page *fscrypt_alloc_bounce_page(struct fscrypt_ctx *ctx,
				       gfp_t gfp_flags)
{
	ctx->w.bounce_page = mempool_alloc(fscrypt_bounce_page_pool, gfp_flags);
	if (ctx->w.bounce_page == NULL)
		return ERR_PTR(-ENOMEM);
	ctx->flags |= FS_CTX_HAS_BOUNCE_BUFFER_FL;
	return ctx->w.bounce_page;
}

/**
 * fscypt_encrypt_page() - Encrypts a page
 * @inode:     The inode for which the encryption should take place
 * @page:      The page to encrypt. Must be locked for bounce-page
 *             encryption.
 * @len:       Length of data to encrypt in @page and encrypted
 *             data in returned page.
 * @offs:      Offset of data within @page and returned
 *             page holding encrypted data.
 * @lblk_num:  Logical block number. This must be unique for multiple
 *             calls with same inode, except when overwriting
 *             previously written data.
 * @gfp_flags: The gfp flag for memory allocation
 *
 * Encrypts @page using the ctx encryption context. Performs encryption
 * either in-place or into a newly allocated bounce page.
 * Called on the page write path.
 *
 * Bounce page allocation is the default.
 * In this case, the contents of @page are encrypted and stored in an
 * allocated bounce page. @page has to be locked and the caller must call
 * fscrypt_restore_control_page() on the returned ciphertext page to
 * release the bounce buffer and the encryption context.
 *
 * In-place encryption is used by setting the FS_CFLG_OWN_PAGES flag in
 * fscrypt_operations. Here, the input-page is returned with its content
 * encrypted.
 *
 * Return: A page with the encrypted content on success. Else, an
 * error value or NULL.
 */
struct page *fscrypt_encrypt_page(const struct inode *inode,
				struct page *page,
				unsigned int len,
				unsigned int offs,
				u64 lblk_num, gfp_t gfp_flags)

{
	struct fscrypt_ctx *ctx;
	struct page *ciphertext_page = page;
	int err;

	if (inode->i_sb->s_cop->flags & FS_CFLG_OWN_PAGES) {
		/* with inplace-encryption we just encrypt the page */
		err = fscrypt_do_page_crypto(inode, FS_ENCRYPT, lblk_num, page,
					     ciphertext_page, len, offs,
					     gfp_flags);
		if (err)
			return ERR_PTR(err);

		return ciphertext_page;
	}
>>>>>>> rebase

	if (WARN_ON_ONCE(!PageLocked(page)))
		return ERR_PTR(-EINVAL);

<<<<<<< HEAD
	if (WARN_ON_ONCE(len <= 0 || !IS_ALIGNED(len | offs, blocksize)))
		return ERR_PTR(-EINVAL);

	ciphertext_page = fscrypt_alloc_bounce_page(gfp_flags);
	if (!ciphertext_page)
		return ERR_PTR(-ENOMEM);

	for (i = offs; i < offs + len; i += blocksize, lblk_num++) {
		err = fscrypt_crypt_block(inode, FS_ENCRYPT, lblk_num,
					  page, ciphertext_page,
					  blocksize, i, gfp_flags);
		if (err) {
			fscrypt_free_bounce_page(ciphertext_page);
			return ERR_PTR(err);
		}
	}
	SetPagePrivate(ciphertext_page);
	set_page_private(ciphertext_page, (unsigned long)page);
	return ciphertext_page;
}
EXPORT_SYMBOL(fscrypt_encrypt_pagecache_blocks);

/**
 * fscrypt_encrypt_block_inplace() - Encrypt a filesystem block in-place
 * @inode:     The inode to which this block belongs
 * @page:      The page containing the block to encrypt
 * @len:       Size of block to encrypt.  Doesn't need to be a multiple of the
 *		fs block size, but must be a multiple of FS_CRYPTO_BLOCK_SIZE.
 * @offs:      Byte offset within @page at which the block to encrypt begins
 * @lblk_num:  Filesystem logical block number of the block, i.e. the 0-based
 *		number of the block within the file
 * @gfp_flags: Memory allocation flags
 *
 * Encrypt a possibly-compressed filesystem block that is located in an
 * arbitrary page, not necessarily in the original pagecache page.  The @inode
 * and @lblk_num must be specified, as they can't be determined from @page.
 *
 * Return: 0 on success; -errno on failure
 */
int fscrypt_encrypt_block_inplace(const struct inode *inode, struct page *page,
				  unsigned int len, unsigned int offs,
				  u64 lblk_num, gfp_t gfp_flags)
{
	return fscrypt_crypt_block(inode, FS_ENCRYPT, lblk_num, page, page,
				   len, offs, gfp_flags);
}
EXPORT_SYMBOL(fscrypt_encrypt_block_inplace);

/**
 * fscrypt_decrypt_pagecache_blocks() - Decrypt filesystem blocks in a pagecache page
 * @page:      The locked pagecache page containing the block(s) to decrypt
 * @len:       Total size of the block(s) to decrypt.  Must be a nonzero
 *		multiple of the filesystem's block size.
 * @offs:      Byte offset within @page of the first block to decrypt.  Must be
 *		a multiple of the filesystem's block size.
 *
 * The specified block(s) are decrypted in-place within the pagecache page,
 * which must still be locked and not uptodate.  Normally, blocksize ==
 * PAGE_SIZE and the whole page is decrypted at once.
 *
 * This is for use by the filesystem's ->readpages() method.
 *
 * Return: 0 on success; -errno on failure
 */
int fscrypt_decrypt_pagecache_blocks(struct page *page, unsigned int len,
				     unsigned int offs)
{
	const struct inode *inode = page->mapping->host;
	const unsigned int blockbits = inode->i_blkbits;
	const unsigned int blocksize = 1 << blockbits;
	u64 lblk_num = ((u64)page->index << (PAGE_SHIFT - blockbits)) +
		       (offs >> blockbits);
	unsigned int i;
	int err;

	if (WARN_ON_ONCE(!PageLocked(page)))
		return -EINVAL;

	if (WARN_ON_ONCE(len <= 0 || !IS_ALIGNED(len | offs, blocksize)))
		return -EINVAL;

#ifdef CONFIG_DDAR
	if (fscrypt_dd_encrypted_inode(inode)) {
		// Invert crypto order. OEM crypto must perform after 3rd party crypto
		return 0;
	}
#endif

	for (i = offs; i < offs + len; i += blocksize, lblk_num++) {
		err = fscrypt_crypt_block(inode, FS_DECRYPT, lblk_num, page,
					  page, blocksize, i, GFP_NOFS);
		if (err)
			return err;
	}
	return 0;
}
EXPORT_SYMBOL(fscrypt_decrypt_pagecache_blocks);

/**
 * fscrypt_decrypt_block_inplace() - Decrypt a filesystem block in-place
 * @inode:     The inode to which this block belongs
 * @page:      The page containing the block to decrypt
 * @len:       Size of block to decrypt.  Doesn't need to be a multiple of the
 *		fs block size, but must be a multiple of FS_CRYPTO_BLOCK_SIZE.
 * @offs:      Byte offset within @page at which the block to decrypt begins
 * @lblk_num:  Filesystem logical block number of the block, i.e. the 0-based
 *		number of the block within the file
 *
 * Decrypt a possibly-compressed filesystem block that is located in an
 * arbitrary page, not necessarily in the original pagecache page.  The @inode
 * and @lblk_num must be specified, as they can't be determined from @page.
 *
 * Return: 0 on success; -errno on failure
 */
int fscrypt_decrypt_block_inplace(const struct inode *inode, struct page *page,
				  unsigned int len, unsigned int offs,
				  u64 lblk_num)
{
	return fscrypt_crypt_block(inode, FS_DECRYPT, lblk_num, page, page,
				   len, offs, GFP_NOFS);
}
EXPORT_SYMBOL(fscrypt_decrypt_block_inplace);
=======
	ctx = fscrypt_get_ctx(inode, gfp_flags);
	if (IS_ERR(ctx))
		return (struct page *)ctx;

	/* The encryption operation will require a bounce page. */
	ciphertext_page = fscrypt_alloc_bounce_page(ctx, gfp_flags);
	if (IS_ERR(ciphertext_page))
		goto errout;

	ctx->w.control_page = page;
	err = fscrypt_do_page_crypto(inode, FS_ENCRYPT, lblk_num,
				     page, ciphertext_page, len, offs,
				     gfp_flags);
	if (err) {
		ciphertext_page = ERR_PTR(err);
		goto errout;
	}
	SetPagePrivate(ciphertext_page);
	set_page_private(ciphertext_page, (unsigned long)ctx);
	lock_page(ciphertext_page);
	return ciphertext_page;

errout:
	fscrypt_release_ctx(ctx);
	return ciphertext_page;
}
EXPORT_SYMBOL(fscrypt_encrypt_page);

/**
 * fscrypt_decrypt_page() - Decrypts a page in-place
 * @inode:     The corresponding inode for the page to decrypt.
 * @page:      The page to decrypt. Must be locked in case
 *             it is a writeback page (FS_CFLG_OWN_PAGES unset).
 * @len:       Number of bytes in @page to be decrypted.
 * @offs:      Start of data in @page.
 * @lblk_num:  Logical block number.
 *
 * Decrypts page in-place using the ctx encryption context.
 *
 * Called from the read completion callback.
 *
 * Return: Zero on success, non-zero otherwise.
 */
int fscrypt_decrypt_page(const struct inode *inode, struct page *page,
			unsigned int len, unsigned int offs, u64 lblk_num)
{
	if (WARN_ON_ONCE(!PageLocked(page) &&
			 !(inode->i_sb->s_cop->flags & FS_CFLG_OWN_PAGES)))
		return -EINVAL;

	return fscrypt_do_page_crypto(inode, FS_DECRYPT, lblk_num, page, page,
				      len, offs, GFP_NOFS);
}
EXPORT_SYMBOL(fscrypt_decrypt_page);

/*
 * Validate dentries in encrypted directories to make sure we aren't potentially
 * caching stale dentries after a key has been added.
 */
static int fscrypt_d_revalidate(struct dentry *dentry, unsigned int flags)
{
	struct dentry *dir;
	int err;
	int valid;

	/*
	 * Plaintext names are always valid, since fscrypt doesn't support
	 * reverting to ciphertext names without evicting the directory's inode
	 * -- which implies eviction of the dentries in the directory.
	 */
	if (!(dentry->d_flags & DCACHE_ENCRYPTED_NAME))
		return 1;

	/*
	 * Ciphertext name; valid if the directory's key is still unavailable.
	 *
	 * Although fscrypt forbids rename() on ciphertext names, we still must
	 * use dget_parent() here rather than use ->d_parent directly.  That's
	 * because a corrupted fs image may contain directory hard links, which
	 * the VFS handles by moving the directory's dentry tree in the dcache
	 * each time ->lookup() finds the directory and it already has a dentry
	 * elsewhere.  Thus ->d_parent can be changing, and we must safely grab
	 * a reference to some ->d_parent to prevent it from being freed.
	 */

	if (flags & LOOKUP_RCU)
		return -ECHILD;

	dir = dget_parent(dentry);
	err = fscrypt_get_encryption_info(d_inode(dir));
	valid = !fscrypt_has_encryption_key(d_inode(dir));
	dput(dir);

	if (err < 0)
		return err;

	return valid;
}

const struct dentry_operations fscrypt_d_ops = {
	.d_revalidate = fscrypt_d_revalidate,
};

void fscrypt_restore_control_page(struct page *page)
{
	struct fscrypt_ctx *ctx;

	ctx = (struct fscrypt_ctx *)page_private(page);
	set_page_private(page, (unsigned long)NULL);
	ClearPagePrivate(page);
	unlock_page(page);
	fscrypt_release_ctx(ctx);
}
EXPORT_SYMBOL(fscrypt_restore_control_page);

static void fscrypt_destroy(void)
{
	struct fscrypt_ctx *pos, *n;

	list_for_each_entry_safe(pos, n, &fscrypt_free_ctxs, free_list)
		kmem_cache_free(fscrypt_ctx_cachep, pos);
	INIT_LIST_HEAD(&fscrypt_free_ctxs);
	mempool_destroy(fscrypt_bounce_page_pool);
	fscrypt_bounce_page_pool = NULL;
}
>>>>>>> rebase

/**
 * fscrypt_initialize() - allocate major buffers for fs encryption.
 * @cop_flags:  fscrypt operations flags
 *
 * We only call this when we start accessing encrypted files, since it
 * results in memory getting allocated that wouldn't otherwise be used.
 *
<<<<<<< HEAD
 * Return: 0 on success; -errno on failure
 */
int fscrypt_initialize(unsigned int cop_flags)
{
	int err = 0;
=======
 * Return: Zero on success, non-zero otherwise.
 */
int fscrypt_initialize(unsigned int cop_flags)
{
	int i, res = -ENOMEM;
>>>>>>> rebase

	/* No need to allocate a bounce page pool if this FS won't use it. */
	if (cop_flags & FS_CFLG_OWN_PAGES)
		return 0;

	mutex_lock(&fscrypt_init_mutex);
	if (fscrypt_bounce_page_pool)
<<<<<<< HEAD
		goto out_unlock;

	err = -ENOMEM;
	fscrypt_bounce_page_pool =
		mempool_create_page_pool(num_prealloc_crypto_pages, 0);
	if (!fscrypt_bounce_page_pool)
		goto out_unlock;

	err = 0;
out_unlock:
	mutex_unlock(&fscrypt_init_mutex);
	return err;
}

void fscrypt_msg(const struct inode *inode, const char *level,
=======
		goto already_initialized;

	for (i = 0; i < num_prealloc_crypto_ctxs; i++) {
		struct fscrypt_ctx *ctx;

		ctx = kmem_cache_zalloc(fscrypt_ctx_cachep, GFP_NOFS);
		if (!ctx)
			goto fail;
		list_add(&ctx->free_list, &fscrypt_free_ctxs);
	}

	fscrypt_bounce_page_pool =
		mempool_create_page_pool(num_prealloc_crypto_pages, 0);
	if (!fscrypt_bounce_page_pool)
		goto fail;

already_initialized:
	mutex_unlock(&fscrypt_init_mutex);
	return 0;
fail:
	fscrypt_destroy();
	mutex_unlock(&fscrypt_init_mutex);
	return res;
}

void fscrypt_msg(struct super_block *sb, const char *level,
>>>>>>> rebase
		 const char *fmt, ...)
{
	static DEFINE_RATELIMIT_STATE(rs, DEFAULT_RATELIMIT_INTERVAL,
				      DEFAULT_RATELIMIT_BURST);
	struct va_format vaf;
	va_list args;

	if (!__ratelimit(&rs))
		return;

	va_start(args, fmt);
	vaf.fmt = fmt;
	vaf.va = &args;
<<<<<<< HEAD
	if (inode)
		printk("%sfscrypt (%s, inode %lu): %pV\n",
		       level, inode->i_sb->s_id, inode->i_ino, &vaf);
=======
	if (sb)
		printk("%sfscrypt (%s): %pV\n", level, sb->s_id, &vaf);
>>>>>>> rebase
	else
		printk("%sfscrypt: %pV\n", level, &vaf);
	va_end(args);
}

/**
 * fscrypt_init() - Set up for fs encryption.
 */
static int __init fscrypt_init(void)
{
<<<<<<< HEAD
	int err = -ENOMEM;

=======
>>>>>>> rebase
	/*
	 * Use an unbound workqueue to allow bios to be decrypted in parallel
	 * even when they happen to complete on the same CPU.  This sacrifices
	 * locality, but it's worthwhile since decryption is CPU-intensive.
	 *
	 * Also use a high-priority workqueue to prioritize decryption work,
	 * which blocks reads from completing, over regular application tasks.
	 */
<<<<<<< HEAD
	int res = -ENOMEM;

=======
>>>>>>> rebase
	fscrypt_read_workqueue = alloc_workqueue("fscrypt_read_queue",
						 WQ_UNBOUND | WQ_HIGHPRI,
						 num_online_cpus());
	if (!fscrypt_read_workqueue)
		goto fail;

<<<<<<< HEAD
	fscrypt_info_cachep = KMEM_CACHE(fscrypt_info, SLAB_RECLAIM_ACCOUNT);
	if (!fscrypt_info_cachep)
		goto fail_free_queue;

	err = fscrypt_init_keyring();
	if (err)
		goto fail_free_info;

#ifdef CONFIG_FSCRYPT_SDP
	res = sdp_crypto_init();
	if (!fscrypt_sdp_init_sdp_info_cachep())
		goto fail_free_info;
#endif
	return 0;

fail_free_info:
	kmem_cache_destroy(fscrypt_info_cachep);
fail_free_queue:
	destroy_workqueue(fscrypt_read_workqueue);
fail:
	return err;
}
late_initcall(fscrypt_init)
=======
	fscrypt_ctx_cachep = KMEM_CACHE(fscrypt_ctx, SLAB_RECLAIM_ACCOUNT);
	if (!fscrypt_ctx_cachep)
		goto fail_free_queue;

	fscrypt_info_cachep = KMEM_CACHE(fscrypt_info, SLAB_RECLAIM_ACCOUNT);
	if (!fscrypt_info_cachep)
		goto fail_free_ctx;

	return 0;

fail_free_ctx:
	kmem_cache_destroy(fscrypt_ctx_cachep);
fail_free_queue:
	destroy_workqueue(fscrypt_read_workqueue);
fail:
	return -ENOMEM;
}
module_init(fscrypt_init)

/**
 * fscrypt_exit() - Shutdown the fs encryption system
 */
static void __exit fscrypt_exit(void)
{
	fscrypt_destroy();

	if (fscrypt_read_workqueue)
		destroy_workqueue(fscrypt_read_workqueue);
	kmem_cache_destroy(fscrypt_ctx_cachep);
	kmem_cache_destroy(fscrypt_info_cachep);

	fscrypt_essiv_cleanup();
}
module_exit(fscrypt_exit);

MODULE_LICENSE("GPL");
>>>>>>> rebase
