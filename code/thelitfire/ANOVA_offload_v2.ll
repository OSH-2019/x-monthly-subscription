; ModuleID = 'ANOVA_offload_v2.c'
source_filename = "ANOVA_offload_v2.c"
target datalayout = "e-m:e-p:64:64-i64:64-n32:64-S128"
target triple = "bpf"

%struct.xdp_md = type { i32, i32, i32, i32, i32 }
%struct.packet_struct = type { i64, i64, [16 x i64] }

@llvm.used = appending global [1 x i8*] [i8* bitcast (i32 (%struct.xdp_md*)* @process_packet to i8*)], section "llvm.metadata"

; Function Attrs: norecurse nounwind
define dso_local i32 @process_packet(%struct.xdp_md* nocapture readonly) #0 section "xdp" {
  %2 = getelementptr inbounds %struct.xdp_md, %struct.xdp_md* %0, i64 0, i32 1
  %3 = load i32, i32* %2, align 4, !tbaa !2
  %4 = zext i32 %3 to i64
  %5 = getelementptr inbounds %struct.xdp_md, %struct.xdp_md* %0, i64 0, i32 0
  %6 = load i32, i32* %5, align 4, !tbaa !7
  %7 = zext i32 %6 to i64
  %8 = inttoptr i64 %7 to i8*
  %9 = getelementptr inbounds i8, i8* %8, i64 186
  %10 = bitcast i8* %9 to %struct.packet_struct*
  %11 = inttoptr i64 %4 to %struct.packet_struct*
  %12 = icmp ugt %struct.packet_struct* %10, %11
  br i1 %12, label %407, label %13

; <label>:13:                                     ; preds = %1
  %14 = getelementptr i8, i8* %8, i64 42
  %15 = bitcast i8* %14 to i64*
  %16 = load i64, i64* %15, align 8, !tbaa !8
  %17 = icmp eq i64 %16, 2408424645786761761
  br i1 %17, label %18, label %407

; <label>:18:                                     ; preds = %13
  %19 = getelementptr inbounds i8, i8* %8, i64 50
  %20 = bitcast i8* %19 to i64*
  %21 = load i64, i64* %20, align 8, !tbaa !11
  %22 = icmp eq i64 %21, 0
  br i1 %22, label %23, label %407

; <label>:23:                                     ; preds = %18
  %24 = getelementptr inbounds i8, i8* %8, i64 58
  %25 = bitcast i8* %24 to i64*
  %26 = load i64, i64* %25, align 8, !tbaa !12
  %27 = icmp eq i64 %26, 0
  br i1 %27, label %40, label %28

; <label>:28:                                     ; preds = %23
  %29 = and i64 %26, 4503599627370495
  %30 = or i64 %29, 4503599627370496
  %31 = lshr i64 %26, 52
  %32 = trunc i64 %31 to i32
  %33 = icmp ult i32 %32, 1065
  %34 = sub nsw i64 1065, %31
  %35 = add nuw nsw i64 %31, 4294966231
  %36 = select i1 %33, i64 %34, i64 %35
  %37 = and i64 %36, 4294967295
  %38 = lshr i64 %30, %37
  %39 = trunc i64 %38 to i32
  br label %40

; <label>:40:                                     ; preds = %23, %28
  %41 = phi i32 [ %39, %28 ], [ 0, %23 ]
  %42 = zext i32 %41 to i64
  %43 = mul nuw i64 %42, %42
  %44 = getelementptr inbounds i8, i8* %8, i64 66
  %45 = bitcast i8* %44 to i64*
  %46 = load i64, i64* %45, align 8, !tbaa !12
  %47 = icmp eq i64 %46, 0
  br i1 %47, label %60, label %48

; <label>:48:                                     ; preds = %40
  %49 = and i64 %46, 4503599627370495
  %50 = or i64 %49, 4503599627370496
  %51 = lshr i64 %46, 52
  %52 = trunc i64 %51 to i32
  %53 = icmp ult i32 %52, 1065
  %54 = sub nsw i64 1065, %51
  %55 = add nuw nsw i64 %51, 4294966231
  %56 = select i1 %53, i64 %54, i64 %55
  %57 = and i64 %56, 4294967295
  %58 = lshr i64 %50, %57
  %59 = trunc i64 %58 to i32
  br label %60

; <label>:60:                                     ; preds = %40, %48
  %61 = phi i32 [ %59, %48 ], [ 0, %40 ]
  %62 = add i32 %61, %41
  %63 = zext i32 %61 to i64
  %64 = mul nuw i64 %63, %63
  %65 = add i64 %64, %43
  %66 = getelementptr inbounds i8, i8* %8, i64 74
  %67 = bitcast i8* %66 to i64*
  %68 = load i64, i64* %67, align 8, !tbaa !12
  %69 = icmp eq i64 %68, 0
  br i1 %69, label %82, label %70

; <label>:70:                                     ; preds = %60
  %71 = and i64 %68, 4503599627370495
  %72 = or i64 %71, 4503599627370496
  %73 = lshr i64 %68, 52
  %74 = trunc i64 %73 to i32
  %75 = icmp ult i32 %74, 1065
  %76 = sub nsw i64 1065, %73
  %77 = add nuw nsw i64 %73, 4294966231
  %78 = select i1 %75, i64 %76, i64 %77
  %79 = and i64 %78, 4294967295
  %80 = lshr i64 %72, %79
  %81 = trunc i64 %80 to i32
  br label %82

; <label>:82:                                     ; preds = %60, %70
  %83 = phi i32 [ %81, %70 ], [ 0, %60 ]
  %84 = add i32 %62, %83
  %85 = zext i32 %83 to i64
  %86 = mul nuw i64 %85, %85
  %87 = add i64 %65, %86
  %88 = getelementptr inbounds i8, i8* %8, i64 82
  %89 = bitcast i8* %88 to i64*
  %90 = load i64, i64* %89, align 8, !tbaa !12
  %91 = icmp eq i64 %90, 0
  br i1 %91, label %104, label %92

; <label>:92:                                     ; preds = %82
  %93 = and i64 %90, 4503599627370495
  %94 = or i64 %93, 4503599627370496
  %95 = lshr i64 %90, 52
  %96 = trunc i64 %95 to i32
  %97 = icmp ult i32 %96, 1065
  %98 = sub nsw i64 1065, %95
  %99 = add nuw nsw i64 %95, 4294966231
  %100 = select i1 %97, i64 %98, i64 %99
  %101 = and i64 %100, 4294967295
  %102 = lshr i64 %94, %101
  %103 = trunc i64 %102 to i32
  br label %104

; <label>:104:                                    ; preds = %82, %92
  %105 = phi i32 [ %103, %92 ], [ 0, %82 ]
  %106 = add i32 %84, %105
  %107 = zext i32 %105 to i64
  %108 = mul nuw i64 %107, %107
  %109 = add i64 %87, %108
  %110 = getelementptr inbounds i8, i8* %8, i64 90
  %111 = bitcast i8* %110 to i64*
  %112 = load i64, i64* %111, align 8, !tbaa !12
  %113 = icmp eq i64 %112, 0
  br i1 %113, label %126, label %114

; <label>:114:                                    ; preds = %104
  %115 = and i64 %112, 4503599627370495
  %116 = or i64 %115, 4503599627370496
  %117 = lshr i64 %112, 52
  %118 = trunc i64 %117 to i32
  %119 = icmp ult i32 %118, 1065
  %120 = sub nsw i64 1065, %117
  %121 = add nuw nsw i64 %117, 4294966231
  %122 = select i1 %119, i64 %120, i64 %121
  %123 = and i64 %122, 4294967295
  %124 = lshr i64 %116, %123
  %125 = trunc i64 %124 to i32
  br label %126

; <label>:126:                                    ; preds = %104, %114
  %127 = phi i32 [ %125, %114 ], [ 0, %104 ]
  %128 = add i32 %106, %127
  %129 = zext i32 %127 to i64
  %130 = mul nuw i64 %129, %129
  %131 = add i64 %109, %130
  %132 = getelementptr inbounds i8, i8* %8, i64 98
  %133 = bitcast i8* %132 to i64*
  %134 = load i64, i64* %133, align 8, !tbaa !12
  %135 = icmp eq i64 %134, 0
  br i1 %135, label %148, label %136

; <label>:136:                                    ; preds = %126
  %137 = and i64 %134, 4503599627370495
  %138 = or i64 %137, 4503599627370496
  %139 = lshr i64 %134, 52
  %140 = trunc i64 %139 to i32
  %141 = icmp ult i32 %140, 1065
  %142 = sub nsw i64 1065, %139
  %143 = add nuw nsw i64 %139, 4294966231
  %144 = select i1 %141, i64 %142, i64 %143
  %145 = and i64 %144, 4294967295
  %146 = lshr i64 %138, %145
  %147 = trunc i64 %146 to i32
  br label %148

; <label>:148:                                    ; preds = %126, %136
  %149 = phi i32 [ %147, %136 ], [ 0, %126 ]
  %150 = add i32 %128, %149
  %151 = zext i32 %149 to i64
  %152 = mul nuw i64 %151, %151
  %153 = add i64 %131, %152
  %154 = getelementptr inbounds i8, i8* %8, i64 106
  %155 = bitcast i8* %154 to i64*
  %156 = load i64, i64* %155, align 8, !tbaa !12
  %157 = icmp eq i64 %156, 0
  br i1 %157, label %170, label %158

; <label>:158:                                    ; preds = %148
  %159 = and i64 %156, 4503599627370495
  %160 = or i64 %159, 4503599627370496
  %161 = lshr i64 %156, 52
  %162 = trunc i64 %161 to i32
  %163 = icmp ult i32 %162, 1065
  %164 = sub nsw i64 1065, %161
  %165 = add nuw nsw i64 %161, 4294966231
  %166 = select i1 %163, i64 %164, i64 %165
  %167 = and i64 %166, 4294967295
  %168 = lshr i64 %160, %167
  %169 = trunc i64 %168 to i32
  br label %170

; <label>:170:                                    ; preds = %148, %158
  %171 = phi i32 [ %169, %158 ], [ 0, %148 ]
  %172 = add i32 %150, %171
  %173 = zext i32 %171 to i64
  %174 = mul nuw i64 %173, %173
  %175 = add i64 %153, %174
  %176 = getelementptr inbounds i8, i8* %8, i64 114
  %177 = bitcast i8* %176 to i64*
  %178 = load i64, i64* %177, align 8, !tbaa !12
  %179 = icmp eq i64 %178, 0
  br i1 %179, label %192, label %180

; <label>:180:                                    ; preds = %170
  %181 = and i64 %178, 4503599627370495
  %182 = or i64 %181, 4503599627370496
  %183 = lshr i64 %178, 52
  %184 = trunc i64 %183 to i32
  %185 = icmp ult i32 %184, 1065
  %186 = sub nsw i64 1065, %183
  %187 = add nuw nsw i64 %183, 4294966231
  %188 = select i1 %185, i64 %186, i64 %187
  %189 = and i64 %188, 4294967295
  %190 = lshr i64 %182, %189
  %191 = trunc i64 %190 to i32
  br label %192

; <label>:192:                                    ; preds = %170, %180
  %193 = phi i32 [ %191, %180 ], [ 0, %170 ]
  %194 = add i32 %172, %193
  %195 = zext i32 %193 to i64
  %196 = mul nuw i64 %195, %195
  %197 = add i64 %175, %196
  %198 = lshr i32 %194, 3
  %199 = mul i32 %198, %198
  %200 = zext i32 %199 to i64
  %201 = getelementptr inbounds i8, i8* %8, i64 122
  %202 = bitcast i8* %201 to i64*
  %203 = load i64, i64* %202, align 8, !tbaa !12
  %204 = icmp eq i64 %203, 0
  br i1 %204, label %217, label %205

; <label>:205:                                    ; preds = %192
  %206 = and i64 %203, 4503599627370495
  %207 = or i64 %206, 4503599627370496
  %208 = lshr i64 %203, 52
  %209 = trunc i64 %208 to i32
  %210 = icmp ult i32 %209, 1065
  %211 = sub nsw i64 1065, %208
  %212 = add nuw nsw i64 %208, 4294966231
  %213 = select i1 %210, i64 %211, i64 %212
  %214 = and i64 %213, 4294967295
  %215 = lshr i64 %207, %214
  %216 = trunc i64 %215 to i32
  br label %217

; <label>:217:                                    ; preds = %192, %205
  %218 = phi i32 [ %216, %205 ], [ 0, %192 ]
  %219 = icmp ult i32 %218, %198
  %220 = sub i32 %218, %198
  %221 = sub i32 %198, %218
  %222 = select i1 %219, i32 %221, i32 %220
  %223 = icmp ugt i32 %222, 101
  %224 = getelementptr inbounds i8, i8* %8, i64 130
  %225 = bitcast i8* %224 to i64*
  %226 = load i64, i64* %225, align 8, !tbaa !12
  %227 = icmp eq i64 %226, 0
  br i1 %227, label %240, label %228

; <label>:228:                                    ; preds = %217
  %229 = and i64 %226, 4503599627370495
  %230 = or i64 %229, 4503599627370496
  %231 = lshr i64 %226, 52
  %232 = trunc i64 %231 to i32
  %233 = icmp ult i32 %232, 1065
  %234 = sub nsw i64 1065, %231
  %235 = add nuw nsw i64 %231, 4294966231
  %236 = select i1 %233, i64 %234, i64 %235
  %237 = and i64 %236, 4294967295
  %238 = lshr i64 %230, %237
  %239 = trunc i64 %238 to i32
  br label %240

; <label>:240:                                    ; preds = %217, %228
  %241 = phi i32 [ %239, %228 ], [ 0, %217 ]
  %242 = icmp ult i32 %241, %198
  %243 = sub i32 %241, %198
  %244 = sub i32 %198, %241
  %245 = select i1 %242, i32 %244, i32 %243
  %246 = icmp ugt i32 %245, 101
  %247 = zext i1 %246 to i64
  %248 = zext i1 %223 to i64
  %249 = add nuw nsw i64 %247, %248
  %250 = getelementptr inbounds i8, i8* %8, i64 138
  %251 = bitcast i8* %250 to i64*
  %252 = load i64, i64* %251, align 8, !tbaa !12
  %253 = icmp eq i64 %252, 0
  br i1 %253, label %266, label %254

; <label>:254:                                    ; preds = %240
  %255 = and i64 %252, 4503599627370495
  %256 = or i64 %255, 4503599627370496
  %257 = lshr i64 %252, 52
  %258 = trunc i64 %257 to i32
  %259 = icmp ult i32 %258, 1065
  %260 = sub nsw i64 1065, %257
  %261 = add nuw nsw i64 %257, 4294966231
  %262 = select i1 %259, i64 %260, i64 %261
  %263 = and i64 %262, 4294967295
  %264 = lshr i64 %256, %263
  %265 = trunc i64 %264 to i32
  br label %266

; <label>:266:                                    ; preds = %240, %254
  %267 = phi i32 [ %265, %254 ], [ 0, %240 ]
  %268 = icmp ult i32 %267, %198
  %269 = sub i32 %267, %198
  %270 = sub i32 %198, %267
  %271 = select i1 %268, i32 %270, i32 %269
  %272 = icmp ugt i32 %271, 101
  %273 = zext i1 %272 to i64
  %274 = add nuw nsw i64 %249, %273
  %275 = getelementptr inbounds i8, i8* %8, i64 146
  %276 = bitcast i8* %275 to i64*
  %277 = load i64, i64* %276, align 8, !tbaa !12
  %278 = icmp eq i64 %277, 0
  br i1 %278, label %291, label %279

; <label>:279:                                    ; preds = %266
  %280 = and i64 %277, 4503599627370495
  %281 = or i64 %280, 4503599627370496
  %282 = lshr i64 %277, 52
  %283 = trunc i64 %282 to i32
  %284 = icmp ult i32 %283, 1065
  %285 = sub nsw i64 1065, %282
  %286 = add nuw nsw i64 %282, 4294966231
  %287 = select i1 %284, i64 %285, i64 %286
  %288 = and i64 %287, 4294967295
  %289 = lshr i64 %281, %288
  %290 = trunc i64 %289 to i32
  br label %291

; <label>:291:                                    ; preds = %266, %279
  %292 = phi i32 [ %290, %279 ], [ 0, %266 ]
  %293 = icmp ult i32 %292, %198
  %294 = sub i32 %292, %198
  %295 = sub i32 %198, %292
  %296 = select i1 %293, i32 %295, i32 %294
  %297 = icmp ugt i32 %296, 101
  %298 = zext i1 %297 to i64
  %299 = add nuw nsw i64 %274, %298
  %300 = getelementptr inbounds i8, i8* %8, i64 154
  %301 = bitcast i8* %300 to i64*
  %302 = load i64, i64* %301, align 8, !tbaa !12
  %303 = icmp eq i64 %302, 0
  br i1 %303, label %316, label %304

; <label>:304:                                    ; preds = %291
  %305 = and i64 %302, 4503599627370495
  %306 = or i64 %305, 4503599627370496
  %307 = lshr i64 %302, 52
  %308 = trunc i64 %307 to i32
  %309 = icmp ult i32 %308, 1065
  %310 = sub nsw i64 1065, %307
  %311 = add nuw nsw i64 %307, 4294966231
  %312 = select i1 %309, i64 %310, i64 %311
  %313 = and i64 %312, 4294967295
  %314 = lshr i64 %306, %313
  %315 = trunc i64 %314 to i32
  br label %316

; <label>:316:                                    ; preds = %291, %304
  %317 = phi i32 [ %315, %304 ], [ 0, %291 ]
  %318 = icmp ult i32 %317, %198
  %319 = sub i32 %317, %198
  %320 = sub i32 %198, %317
  %321 = select i1 %318, i32 %320, i32 %319
  %322 = icmp ugt i32 %321, 101
  %323 = zext i1 %322 to i64
  %324 = add nuw nsw i64 %299, %323
  %325 = getelementptr inbounds i8, i8* %8, i64 162
  %326 = bitcast i8* %325 to i64*
  %327 = load i64, i64* %326, align 8, !tbaa !12
  %328 = icmp eq i64 %327, 0
  br i1 %328, label %341, label %329

; <label>:329:                                    ; preds = %316
  %330 = and i64 %327, 4503599627370495
  %331 = or i64 %330, 4503599627370496
  %332 = lshr i64 %327, 52
  %333 = trunc i64 %332 to i32
  %334 = icmp ult i32 %333, 1065
  %335 = sub nsw i64 1065, %332
  %336 = add nuw nsw i64 %332, 4294966231
  %337 = select i1 %334, i64 %335, i64 %336
  %338 = and i64 %337, 4294967295
  %339 = lshr i64 %331, %338
  %340 = trunc i64 %339 to i32
  br label %341

; <label>:341:                                    ; preds = %316, %329
  %342 = phi i32 [ %340, %329 ], [ 0, %316 ]
  %343 = icmp ult i32 %342, %198
  %344 = sub i32 %342, %198
  %345 = sub i32 %198, %342
  %346 = select i1 %343, i32 %345, i32 %344
  %347 = icmp ugt i32 %346, 101
  %348 = zext i1 %347 to i64
  %349 = add nuw nsw i64 %324, %348
  %350 = getelementptr inbounds i8, i8* %8, i64 170
  %351 = bitcast i8* %350 to i64*
  %352 = load i64, i64* %351, align 8, !tbaa !12
  %353 = icmp eq i64 %352, 0
  br i1 %353, label %366, label %354

; <label>:354:                                    ; preds = %341
  %355 = and i64 %352, 4503599627370495
  %356 = or i64 %355, 4503599627370496
  %357 = lshr i64 %352, 52
  %358 = trunc i64 %357 to i32
  %359 = icmp ult i32 %358, 1065
  %360 = sub nsw i64 1065, %357
  %361 = add nuw nsw i64 %357, 4294966231
  %362 = select i1 %359, i64 %360, i64 %361
  %363 = and i64 %362, 4294967295
  %364 = lshr i64 %356, %363
  %365 = trunc i64 %364 to i32
  br label %366

; <label>:366:                                    ; preds = %341, %354
  %367 = phi i32 [ %365, %354 ], [ 0, %341 ]
  %368 = icmp ult i32 %367, %198
  %369 = sub i32 %367, %198
  %370 = sub i32 %198, %367
  %371 = select i1 %368, i32 %370, i32 %369
  %372 = icmp ugt i32 %371, 101
  %373 = zext i1 %372 to i64
  %374 = add nuw nsw i64 %349, %373
  %375 = getelementptr inbounds i8, i8* %8, i64 178
  %376 = bitcast i8* %375 to i64*
  %377 = load i64, i64* %376, align 8, !tbaa !12
  %378 = icmp eq i64 %377, 0
  br i1 %378, label %391, label %379

; <label>:379:                                    ; preds = %366
  %380 = and i64 %377, 4503599627370495
  %381 = or i64 %380, 4503599627370496
  %382 = lshr i64 %377, 52
  %383 = trunc i64 %382 to i32
  %384 = icmp ult i32 %383, 1065
  %385 = sub nsw i64 1065, %382
  %386 = add nuw nsw i64 %382, 4294966231
  %387 = select i1 %384, i64 %385, i64 %386
  %388 = and i64 %387, 4294967295
  %389 = lshr i64 %381, %388
  %390 = trunc i64 %389 to i32
  br label %391

; <label>:391:                                    ; preds = %366, %379
  %392 = phi i32 [ %390, %379 ], [ 0, %366 ]
  %393 = icmp ult i32 %392, %198
  %394 = sub i32 %392, %198
  %395 = sub i32 %198, %392
  %396 = select i1 %393, i32 %395, i32 %394
  %397 = icmp ugt i32 %396, 101
  %398 = zext i1 %397 to i64
  %399 = add i64 %374, %398
  %400 = and i64 %399, 65535
  %401 = mul nuw nsw i64 %400, 10404
  %402 = shl nuw nsw i64 %200, 3
  %403 = sub i64 %197, %402
  %404 = and i64 %403, -8
  %405 = icmp ugt i64 %401, %404
  %406 = select i1 %405, i64 1, i64 2
  store i64 %406, i64* %20, align 8, !tbaa !11
  br label %407

; <label>:407:                                    ; preds = %18, %13, %1, %391
  ret i32 2
}

attributes #0 = { norecurse nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 7.0.1 (Fedora 7.0.1-6.fc29)"}
!2 = !{!3, !4, i64 4}
!3 = !{!"xdp_md", !4, i64 0, !4, i64 4, !4, i64 8, !4, i64 12, !4, i64 16}
!4 = !{!"int", !5, i64 0}
!5 = !{!"omnipotent char", !6, i64 0}
!6 = !{!"Simple C/C++ TBAA"}
!7 = !{!3, !4, i64 0}
!8 = !{!9, !10, i64 0}
!9 = !{!"packet_struct", !10, i64 0, !10, i64 8, !5, i64 16}
!10 = !{!"long long", !5, i64 0}
!11 = !{!9, !10, i64 8}
!12 = !{!10, !10, i64 0}
