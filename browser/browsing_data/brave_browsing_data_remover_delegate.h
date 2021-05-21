/* Copyright (c) 2020 The adrbrowsiel Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef adrbrowsiel_BROWSER_BROWSING_DATA_adrbrowsiel_BROWSING_DATA_REMOVER_DELEGATE_H_
#define adrbrowsiel_BROWSER_BROWSING_DATA_adrbrowsiel_BROWSING_DATA_REMOVER_DELEGATE_H_

#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "adrbrowsiel/components/ipfs/buildflags/buildflags.h"
#include "chrome/browser/browsing_data/chrome_browsing_data_remover_delegate.h"

namespace base {
class Process;
}  // namespace base

namespace content_settings {
class adrbrowsielPrefProvider;
}  // namespace content_settings

class Profile;

class adrbrowsielBrowsingDataRemoverDelegate
    : public ChromeBrowsingDataRemoverDelegate {
 public:
  explicit adrbrowsielBrowsingDataRemoverDelegate(
      content::BrowserContext* browser_context);
  ~adrbrowsielBrowsingDataRemoverDelegate() override;

  adrbrowsielBrowsingDataRemoverDelegate(
      const adrbrowsielBrowsingDataRemoverDelegate&) = delete;
  adrbrowsielBrowsingDataRemoverDelegate operator=(
      const adrbrowsielBrowsingDataRemoverDelegate&) = delete;

 private:
  FRIEND_TEST_ALL_PREFIXES(adrbrowsielBrowsingDataRemoverDelegateTest,
                           ShieldsSettingsClearTest);

  // ChromeBrowsingDataRemoverDelegate overrides:
  void RemoveEmbedderData(const base::Time& delete_begin,
                          const base::Time& delete_end,
                          uint64_t remove_mask,
                          content::BrowsingDataFilterBuilder* filter_builder,
                          uint64_t origin_type_mask,
                          base::OnceCallback<void(uint64_t)> callback)
                          override;

  void ClearShieldsSettings(base::Time begin_time, base::Time end_time);
#if BUILDFLAG(IPFS_ENABLED)
  void ClearIPFSCache();
  void WaitForIPFSRepoGC(base::Process process);
#endif

  Profile* profile_;
  base::WeakPtrFactory<adrbrowsielBrowsingDataRemoverDelegate> weak_ptr_factory_{
      this};
};

#endif  // adrbrowsiel_BROWSER_BROWSING_DATA_adrbrowsiel_BROWSING_DATA_REMOVER_DELEGATE_H_
