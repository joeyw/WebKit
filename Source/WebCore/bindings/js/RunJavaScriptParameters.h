/*
 * Copyright (C) 2019-2023 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include <JavaScriptCore/SourceProvider.h>

#include <wtf/HashMap.h>
#include <wtf/URL.h>
#include <wtf/Vector.h>
#include <wtf/text/WTFString.h>

namespace WebCore {

enum class RunAsAsyncFunction : bool { No, Yes };
enum class ForceUserGesture : bool { No, Yes };
enum class RemoveTransientActivation : bool { No, Yes };

using ArgumentWireBytesMap = HashMap<String, Vector<uint8_t>>;

struct RunJavaScriptParameters {
    RunJavaScriptParameters(String&& source, JSC::SourceTaintedOrigin taintedness, URL&& sourceURL, RunAsAsyncFunction runAsAsyncFunction, std::optional<ArgumentWireBytesMap>&& arguments, ForceUserGesture forceUserGesture, RemoveTransientActivation removeTransientActivation)
        : source(WTFMove(source))
        , sourceURL(WTFMove(sourceURL))
        , taintedness(taintedness)
        , runAsAsyncFunction(runAsAsyncFunction)
        , arguments(WTFMove(arguments))
        , forceUserGesture(forceUserGesture)
        , removeTransientActivation(removeTransientActivation)
    {
    }

    RunJavaScriptParameters(const String& source, JSC::SourceTaintedOrigin taintedness, URL&& sourceURL, bool runAsAsyncFunction, std::optional<ArgumentWireBytesMap>&& arguments, bool forceUserGesture, RemoveTransientActivation removeTransientActivation)
        : source(source)
        , sourceURL(WTFMove(sourceURL))
        , taintedness(taintedness)
        , runAsAsyncFunction(runAsAsyncFunction ? RunAsAsyncFunction::Yes : RunAsAsyncFunction::No)
        , arguments(WTFMove(arguments))
        , forceUserGesture(forceUserGesture ? ForceUserGesture::Yes : ForceUserGesture::No)
        , removeTransientActivation(removeTransientActivation)
    {
    }

    RunJavaScriptParameters(String&& source, JSC::SourceTaintedOrigin taintedness, URL&& sourceURL, bool runAsAsyncFunction, std::optional<ArgumentWireBytesMap>&& arguments, bool forceUserGesture, RemoveTransientActivation removeTransientActivation)
        : source(WTFMove(source))
        , sourceURL(WTFMove(sourceURL))
        , taintedness(taintedness)
        , runAsAsyncFunction(runAsAsyncFunction ? RunAsAsyncFunction::Yes : RunAsAsyncFunction::No)
        , arguments(WTFMove(arguments))
        , forceUserGesture(forceUserGesture ? ForceUserGesture::Yes : ForceUserGesture::No)
        , removeTransientActivation(removeTransientActivation)
    {
    }

    String source;
    URL sourceURL;
    JSC::SourceTaintedOrigin taintedness;
    RunAsAsyncFunction runAsAsyncFunction;
    std::optional<ArgumentWireBytesMap> arguments;
    ForceUserGesture forceUserGesture;
    RemoveTransientActivation removeTransientActivation;

    template<typename Encoder>
    void encode(Encoder& encoder) const
    {
        encoder << source << sourceURL << taintedness << runAsAsyncFunction << arguments << forceUserGesture << removeTransientActivation;
    }

    template<typename Decoder>
    static std::optional<RunJavaScriptParameters> decode(Decoder& decoder)
    {
        String source;
        if (!decoder.decode(source))
            return std::nullopt;

        URL sourceURL;
        if (!decoder.decode(sourceURL))
            return std::nullopt;

        JSC::SourceTaintedOrigin taintedness;
        if (!decoder.decode(taintedness))
            return std::nullopt;

        RunAsAsyncFunction runAsAsyncFunction;
        if (!decoder.decode(runAsAsyncFunction))
            return std::nullopt;

        std::optional<ArgumentWireBytesMap> arguments;
        if (!decoder.decode(arguments))
            return std::nullopt;

        ForceUserGesture forceUserGesture;
        if (!decoder.decode(forceUserGesture))
            return std::nullopt;

        RemoveTransientActivation removeTransientActivation;
        if (!decoder.decode(removeTransientActivation))
            return std::nullopt;

        return { RunJavaScriptParameters { WTFMove(source), taintedness, WTFMove(sourceURL), runAsAsyncFunction, WTFMove(arguments), forceUserGesture, removeTransientActivation } };
    }
};

} // namespace WebCore
